#include "llvm/Transforms/Vectorize/VectorPredication.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/VectorBuilder.h"
#include "llvm/Transforms/Utils/Local.h"

#define DEBUG_TYPE "vector-predication"
STATISTIC(Transforms, "Number of full-length -> evl vector transformation.");

using namespace llvm;

// Map each instruction to its uses and save all memory writing vector
// predicated instructions found in the basic block.
void VectorPredicationPass::analyseBasicBlock(BasicBlock &BB,
                                              BlockData &BBInfo) {
  // Store all memory accessing instructions: all these instructions have to be
  // chained, so that their relative order can be preserved when rewriting the
  // basic block.
  SmallVector<Instruction *> ToBeChainedInstructions;

  for (Instruction &I : BB) {
    if (isa<PHINode>(I) || I.isTerminator())
      continue;

    SmallPtrSet<Instruction *, 4> IUsers;
    for (User *IU : I.users()) {
      assert(isa<Instruction>(IU) && "Unexpected behaviour.");
      auto *IUInst = cast<Instruction>(IU);
      if (IUInst->getParent() != I.getParent())
        continue;
      if (isa<PHINode>(IUInst) || IUInst->isTerminator())
        continue;

      IUsers.insert(IUInst);
    }
    BBInfo.TopologicalGraph.insert({&I, IUsers});

    if (I.mayReadOrWriteMemory() || I.mayHaveSideEffects())
      ToBeChainedInstructions.push_back(&I);

    if (auto *CI = dyn_cast<CallInst>(&I)) {
      if (auto *CF = CI->getCalledFunction()) {
        Intrinsic::ID ID = CF->getIntrinsicID();
        if (ID == Intrinsic::vp_store || ID == Intrinsic::vp_scatter) {
          BBInfo.MemoryWritingVPInstructions.push_back(&I);
        }
      }
    }
  }

  if (ToBeChainedInstructions.size() > 1) {
    for (unsigned I = 0; I < ToBeChainedInstructions.size() - 2; I++) {
      auto *Parent = ToBeChainedInstructions[I];
      auto *Child = ToBeChainedInstructions[I + 1];
      BBInfo.TopologicalGraph[Parent].insert(Child);
    }
  }
}

namespace {
void findCandidateVectorOperation(BasicBlock &BB, Value *Op, Value *Mask,
                                  Value *EVL, BlockData &BBInfo) {
  auto *OpInst = dyn_cast<Instruction>(Op);
  if (!OpInst)
    return;

  if (OpInst->getParent() != &BB)
    return;

  Intrinsic::ID VPID = Intrinsic::not_intrinsic;
  unsigned Opcode = OpInst->getOpcode();
  if (Opcode == Instruction::Call) {
    if (auto *CF = cast<CallInst>(OpInst)->getCalledFunction())
      VPID = VPIntrinsic::getForIntrinsicID(CF->getIntrinsicID());
  } else
    VPID = VPIntrinsic::getForOpcode(OpInst->getOpcode());
  if (VPID == Intrinsic::not_intrinsic)
    return;

  // If the instruction is already present in the map, it means it was already
  // visited starting from a previous memory writting vp operation.
  if (!BBInfo.VecOpsToTransform
           .insert(std::make_pair(OpInst, std::make_pair(Mask, EVL)))
           .second) {
    // We need to check if new mask and evl values differ from the old ones:
    // - if they are the same, then there is nothing to do;
    // - if only the mask differ, we use an allones mask;
    // - otherwise, we remove the instruction from the map (i.e., no
    //   transformation should happen)
    // NOTE: maybe, instead of giving up, we could split case 3 in two
    // more cases: if only EVLs differs, we use VLMAX with the mask; if both
    // mask and EVL differ, we use an allones mask and VLMAX (even if
    // semantically it means not doing anything).
    auto It = BBInfo.VecOpsToTransform.find(OpInst);
    assert(It != BBInfo.VecOpsToTransform.end());
    Value *OldMask, *OldEVL;
    std::tie(OldMask, OldEVL) = It->second;

    if (Mask == OldMask && EVL == OldEVL)
      return;

    BBInfo.VecOpsToTransform.erase(OpInst);
    if (EVL == OldEVL) {
      BBInfo.VecOpsToTransform.insert(
          std::make_pair(OpInst, std::make_pair(nullptr, EVL)));
    }
  }

  // Recursively visit OpInst operands.
  switch (VPID) {
  default:
    for (auto *OpVal : OpInst->operand_values())
      findCandidateVectorOperation(BB, OpVal, Mask, EVL, BBInfo);
    break;
  case Intrinsic::vp_select: {
    auto CanBackPropagateCondOpAsMask = [&](Value *CondOp) -> bool {
      if (!CondOp->getType()->isVectorTy())
        return false;

      auto *CondInstr = dyn_cast<Instruction>(CondOp);
      if (!CondInstr)
        return false;
      if (CondInstr->getParent() != &BB)
        return false;
      if (auto *ALM = dyn_cast<CallInst>(CondInstr);
          ALM && ALM->getCalledFunction()->getIntrinsicID() ==
                     Intrinsic::get_active_lane_mask)
        return false;

      return true;
    };

    Value *Cond = OpInst->getOperand(0);
    Value *TrueOp = OpInst->getOperand(1);
    Value *FalseOp = OpInst->getOperand(2);
    // If the condition argument is a vector, we backpropagate it as mask
    // for the true branch and its negation as mask for the false one.
    if (CanBackPropagateCondOpAsMask(Cond)) {
      auto *CondInstr = cast<Instruction>(Cond);
      IRBuilder<> Builder(CondInstr);
      auto *CondNot = cast<Instruction>(Builder.CreateNot(Cond));
      SmallPtrSet<Instruction *, 4> CondNotUsers;
      BBInfo.TopologicalGraph.insert({CondNot, CondNotUsers});
      BBInfo.TopologicalGraph[CondInstr].insert(CondNot);

      findCandidateVectorOperation(BB, Cond, nullptr, EVL, BBInfo);
      findCandidateVectorOperation(BB, CondNot, nullptr, EVL, BBInfo);

      findCandidateVectorOperation(BB, TrueOp, Cond, EVL, BBInfo);
      findCandidateVectorOperation(BB, FalseOp, CondNot, EVL, BBInfo);
    } else {
      findCandidateVectorOperation(BB, TrueOp, nullptr, EVL, BBInfo);
      findCandidateVectorOperation(BB, FalseOp, nullptr, EVL, BBInfo);
    }
    break;
  }
  }
}
} // namespace

// For each vector predicated memory writing operation of the basic block, go
// back to the stored vector defining instruction and verify it is a vector
// operation. Add it to the list of instructions to be transformed into vector
// predicated ones, then recursively repeat the process for its vector
// arguments.
void VectorPredicationPass::findCandidateVectorOperations(BasicBlock &BB,
                                                          BlockData &BBInfo) {
  if (BBInfo.MemoryWritingVPInstructions.empty())
    return;

  for (Instruction *I : BBInfo.MemoryWritingVPInstructions) {
    assert(I->getParent() == &BB && "This is not the right basic block");
    auto *VPI = cast<VPIntrinsic>(I);
    Value *StoredOperand = VPI->getMemoryDataParam();
    Value *MaskOperand = VPI->getMaskParam();
    Value *EVLOperand = VPI->getVectorLengthParam();
    // First, visit the mask operand (assigning an allones mask to this branch)
    // and only then visit the stored operand.
    findCandidateVectorOperation(BB, MaskOperand, nullptr, EVLOperand, BBInfo);
    findCandidateVectorOperation(BB, StoredOperand, MaskOperand, EVLOperand,
                                 BBInfo);
  }
}

// Add the candidates as users of the mask and of the evl linked to each of
// them, but only if they belong to the same basic block.
void VectorPredicationPass::addNewUsersToMasksAndEVLs(BasicBlock &BB,
                                                      BlockData &BBInfo) {
  if (BBInfo.VecOpsToTransform.empty())
    return;

  for (auto [K, V] : BBInfo.VecOpsToTransform) {
    if (auto *MaskInst = dyn_cast_if_present<Instruction>(V.first);
        MaskInst && MaskInst->getParent() == &BB)
      BBInfo.TopologicalGraph[MaskInst].insert(K);
    if (auto *EVLInst = dyn_cast<Instruction>(V.second);
        EVLInst && EVLInst->getParent() == &BB)
      BBInfo.TopologicalGraph[EVLInst].insert(K);
  }
}

// Topologically sort, preserving as much as possible the original order.
void VectorPredicationPass::buildNewBasicBlockSchedule(BasicBlock &BB,
                                                       BlockData &BBInfo) {
  if (BBInfo.VecOpsToTransform.empty())
    return;

  while (!BBInfo.TopologicalGraph.empty()) {
    Instruction *Inst = nullptr;
    for (auto B = BBInfo.TopologicalGraph.rbegin(),
              E = BBInfo.TopologicalGraph.rend();
         B != E; B++) {
      if (B->second.empty()) {
        Inst = B->first;
        break;
      }
    }
    assert(Inst && "Failed to empty topological graph!");

    BBInfo.NewBBReverseOrder.push_back(Inst);
    BBInfo.TopologicalGraph.erase(Inst);

    for (auto B = BBInfo.TopologicalGraph.begin(),
              E = BBInfo.TopologicalGraph.end();
         B != E; B++) {
      B->second.erase(Inst);
    }
  }
}

// Modify the basic block based on the topological order generated.
void VectorPredicationPass::emitNewBasicBlockSchedule(BasicBlock &BB,
                                                      BlockData &BBInfo) {
  if (BBInfo.VecOpsToTransform.empty())
    return;

  Instruction *InsertPoint = BB.getTerminator();
  for (Instruction *I : BBInfo.NewBBReverseOrder) {
    I->moveBefore(InsertPoint);
    InsertPoint = I;
  }
}

// Transform candidates to vector predicated instructions.
void VectorPredicationPass::transformCandidateVectorOperations(
    BasicBlock &BB, BlockData &BBInfo) {
  if (BBInfo.VecOpsToTransform.empty())
    return;

  for (auto [I, P] : BBInfo.VecOpsToTransform) {
    Value *Mask, *EVL;
    std::tie(Mask, EVL) = P;

    IRBuilder<> Builder(I);
    unsigned int OpcodeOrIID = I->getOpcode();
    Type *RetTy = I->getType();
    SmallVector<Value *> Operands(I->value_op_begin(), I->value_op_end());
    bool IsCall = false;
    switch (OpcodeOrIID) {
    case Instruction::Call: {
      Operands.clear();
      auto *CI = cast<CallInst>(I);
      for (auto &Op : CI->operands()) {
        if (Op == CI->getCalledOperand())
          continue;
        Operands.push_back(Op.get());
      }
      OpcodeOrIID = CI->getCalledFunction()->getIntrinsicID();
      IsCall = true;
      break;
    }
    case Instruction::FCmp:
    case Instruction::ICmp: {
      Operands.clear();
      auto *CmpI = cast<CmpInst>(I);
      Value *PredOp = MetadataAsValue::get(
          Builder.getContext(),
          MDString::get(Builder.getContext(),
                        CmpInst::getPredicateName(CmpI->getPredicate())));
      Operands = {CmpI->getOperand(0), CmpI->getOperand(1), PredOp};
      break;
    }
    case Instruction::Select: {
      if (!I->getOperand(0)->getType()->isVectorTy()) {
        Operands.clear();
        Value *Op1 = I->getOperand(1);
        Value *Op2 = I->getOperand(2);
        Value *Cond = Builder.CreateVectorSplat(
            cast<VectorType>(Op1->getType())->getElementCount(),
            I->getOperand(0), "select.cond.splat");
        Operands = {Cond, Op1, Op2};
      } else if (auto *ALM = dyn_cast<CallInst>(I->getOperand(0));
            ALM && ALM->getCalledFunction()->getIntrinsicID() ==
                       Intrinsic::get_active_lane_mask) {
        // Ignore the select: the vector length operand already takes care of
        // keeping track of the active elements.
        I->replaceAllUsesWith(I->getOperand(1));
        // If a select instruction is to be deleted, replace
        // its uses as mask in VecOpsToTransform as well.
        for (auto Iter = BBInfo.VecOpsToTransform.begin();
             Iter != BBInfo.VecOpsToTransform.end(); ++Iter) {
          if (Iter->second.first == I)
            Iter->second.first = I->getOperand(1);
        }
        OldInstructionsToRemove.insert(std::make_pair(I, nullptr));

        continue;
      }
      break;
    }
    default:
      break;
    }

    if (!Mask)
      // nullptr means unmasked operation, hence we use an all-ones mask.
      Mask = ConstantInt::getTrue(RetTy->getWithNewType(Builder.getInt1Ty()));

    VectorBuilder VecBuilder(Builder);
    VecBuilder.setMask(Mask).setEVL(EVL);
    Value *NewVPOp = nullptr;
    if (IsCall)
      NewVPOp = VecBuilder.createVectorInstructionFromIntrinsicID(
          OpcodeOrIID, RetTy, Operands, "vp.op");
    else
      NewVPOp = VecBuilder.createVectorInstructionFromOpcode(OpcodeOrIID, RetTy,
                                                             Operands, "vp.op");

    Transforms++; // Stats
    OldInstructionsToRemove.insert(std::make_pair(I, NewVPOp));
  }
}

// Remove old instructions, if possible.
void VectorPredicationPass::removeOldInstructions() {
  for (auto [I, NewVPOp] : OldInstructionsToRemove) {
    if (NewVPOp)
      I->replaceAllUsesWith(NewVPOp);
    if (isInstructionTriviallyDead(I))
      I->eraseFromParent();
  }
}

PreservedAnalyses VectorPredicationPass::run(Function &F,
                                             FunctionAnalysisManager &AM) {
  assert(OldInstructionsToRemove.empty() &&
         "Map should be cleared at the end of each run of the pass.");

  for (BasicBlock &BB : F) {
    BlockData BBInfo;

    analyseBasicBlock(BB, BBInfo);
    findCandidateVectorOperations(BB, BBInfo);
    addNewUsersToMasksAndEVLs(BB, BBInfo);
    buildNewBasicBlockSchedule(BB, BBInfo);
    emitNewBasicBlockSchedule(BB, BBInfo);
    transformCandidateVectorOperations(BB, BBInfo);
  }

  removeOldInstructions();
  OldInstructionsToRemove.clear();

  // TODO: think about which analysis are preserved.
  return PreservedAnalyses::none();
}
