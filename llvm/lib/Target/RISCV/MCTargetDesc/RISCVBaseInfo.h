//===-- RISCVBaseInfo.h - Top level definitions for RISCV MC ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone enum definitions for the RISCV target
// useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_RISCV_MCTARGETDESC_RISCVBASEINFO_H
#define LLVM_LIB_TARGET_RISCV_MCTARGETDESC_RISCVBASEINFO_H

#include "MCTargetDesc/RISCVMCTargetDesc.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCRegister.h"
#include "llvm/MC/SubtargetFeature.h"

namespace llvm {

// RISCVII - This namespace holds all of the target specific flags that
// instruction info tracks. All definitions must match RISCVInstrFormats.td.
namespace RISCVII {
enum {
  InstFormatPseudo = 0,
  InstFormatR = 1,
  InstFormatR4 = 2,
  InstFormatI = 3,
  InstFormatS = 4,
  InstFormatB = 5,
  InstFormatU = 6,
  InstFormatJ = 7,
  InstFormatCR = 8,
  InstFormatCI = 9,
  InstFormatCSS = 10,
  InstFormatCIW = 11,
  InstFormatCL = 12,
  InstFormatCS = 13,
  InstFormatCA = 14,
  InstFormatCB = 15,
  InstFormatCJ = 16,
  InstFormatOther = 17,

  InstFormatMask = 31,

  ConstraintShift = 5,
  ConstraintMask = 0b111 << ConstraintShift,

  VLMulShift = ConstraintShift + 3,
  VLMulMask = 0b111 << VLMulShift,

  // Do we need to add a dummy mask op when converting RVV Pseudo to MCInst.
  HasDummyMaskOpShift = VLMulShift + 3,
  HasDummyMaskOpMask = 1 << HasDummyMaskOpShift,

  // Force a tail agnostic policy even this instruction has a tied destination.
  ForceTailAgnosticShift = HasDummyMaskOpShift + 1,
  ForceTailAgnosticMask = 1 << ForceTailAgnosticShift,

  // Does this instruction have a merge operand that must be removed when
  // converting to MCInst. It will be the first explicit use operand. Used by
  // RVV Pseudos.
  HasMergeOpShift = ForceTailAgnosticShift + 1,
  HasMergeOpMask = 1 << HasMergeOpShift,

  // Does this instruction have a SEW operand. It will be the last explicit
  // operand. Used by RVV Pseudos.
  HasSEWOpShift = HasMergeOpShift + 1,
  HasSEWOpMask = 1 << HasSEWOpShift,

  // Does this instruction have a VL operand. It will be the second to last
  // explicit operand. Used by RVV Pseudos.
  HasVLOpShift = HasSEWOpShift + 1,
  HasVLOpMask = 1 << HasVLOpShift,
};

// Match with the definitions in RISCVInstrFormatsV.td
enum RVVConstraintType {
  NoConstraint = 0,
  VS2Constraint = 0b001,
  VS1Constraint = 0b010,
  VMConstraint = 0b100,
};

// RISC-V Specific Machine Operand Flags
enum {
  MO_None = 0,
  MO_CALL = 1,
  MO_PLT = 2,
  MO_LO = 3,
  MO_HI = 4,
  MO_PCREL_LO = 5,
  MO_PCREL_HI = 6,
  MO_GOT_HI = 7,
  MO_TPREL_LO = 8,
  MO_TPREL_HI = 9,
  MO_TPREL_ADD = 10,
  MO_TLS_GOT_HI = 11,
  MO_TLS_GD_HI = 12,

  // Used to differentiate between target-specific "direct" flags and "bitmask"
  // flags. A machine operand can only have one "direct" flag, but can have
  // multiple "bitmask" flags.
  MO_DIRECT_FLAG_MASK = 15
};
} // namespace RISCVII

namespace RISCVOp {
enum OperandType : unsigned {
  OPERAND_FIRST_RISCV_IMM = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_UIMM4 = OPERAND_FIRST_RISCV_IMM,
  OPERAND_UIMM5,
  OPERAND_UIMM12,
  OPERAND_SIMM12,
  OPERAND_UIMM20,
  OPERAND_UIMMLOG2XLEN,
  OPERAND_LAST_RISCV_IMM = OPERAND_UIMMLOG2XLEN
};
} // namespace RISCVOp

// Describes the predecessor/successor bits used in the FENCE instruction.
namespace RISCVFenceField {
enum FenceField {
  I = 8,
  O = 4,
  R = 2,
  W = 1,
};
}

// Describes the supported floating point rounding mode encodings.
namespace RISCVFPRndMode {
enum RoundingMode {
  RNE = 0,
  RTZ = 1,
  RDN = 2,
  RUP = 3,
  RMM = 4,
  DYN = 7,
  Invalid,
};

inline static StringRef roundingModeToString(RoundingMode RndMode) {
  switch (RndMode) {
  default:
    llvm_unreachable("Unknown floating point rounding mode");
  case RISCVFPRndMode::RNE:
    return "rne";
  case RISCVFPRndMode::RTZ:
    return "rtz";
  case RISCVFPRndMode::RDN:
    return "rdn";
  case RISCVFPRndMode::RUP:
    return "rup";
  case RISCVFPRndMode::RMM:
    return "rmm";
  case RISCVFPRndMode::DYN:
    return "dyn";
  }
}

inline static RoundingMode stringToRoundingMode(StringRef Str) {
  return StringSwitch<RoundingMode>(Str)
      .Case("rne", RISCVFPRndMode::RNE)
      .Case("rtz", RISCVFPRndMode::RTZ)
      .Case("rdn", RISCVFPRndMode::RDN)
      .Case("rup", RISCVFPRndMode::RUP)
      .Case("rmm", RISCVFPRndMode::RMM)
      .Case("dyn", RISCVFPRndMode::DYN)
      .Default(RISCVFPRndMode::Invalid);
}

inline static bool isValidRoundingMode(unsigned Mode) {
  switch (Mode) {
  default:
    return false;
  case RISCVFPRndMode::RNE:
  case RISCVFPRndMode::RTZ:
  case RISCVFPRndMode::RDN:
  case RISCVFPRndMode::RUP:
  case RISCVFPRndMode::RMM:
  case RISCVFPRndMode::DYN:
    return true;
  }
}
} // namespace RISCVFPRndMode

namespace RISCVSysReg {
struct SysReg {
  const char *Name;
  unsigned Encoding;
  const char *AltName;
  // FIXME: add these additional fields when needed.
  // Privilege Access: Read, Write, Read-Only.
  // unsigned ReadWrite;
  // Privilege Mode: User, System or Machine.
  // unsigned Mode;
  // Check field name.
  // unsigned Extra;
  // Register number without the privilege bits.
  // unsigned Number;
  FeatureBitset FeaturesRequired;
  bool isRV32Only;

  bool haveRequiredFeatures(FeatureBitset ActiveFeatures) const {
    // Not in 32-bit mode.
    if (isRV32Only && ActiveFeatures[RISCV::Feature64Bit])
      return false;
    // No required feature associated with the system register.
    if (FeaturesRequired.none())
      return true;
    return (FeaturesRequired & ActiveFeatures) == FeaturesRequired;
  }
};

#define GET_SysRegsList_DECL
#include "RISCVGenSearchableTables.inc"
} // end namespace RISCVSysReg

namespace RISCVABI {

enum ABI {
  ABI_ILP32,
  ABI_ILP32F,
  ABI_ILP32D,
  ABI_ILP32E,
  ABI_LP64,
  ABI_LP64F,
  ABI_LP64D,
  ABI_Unknown,
};

// Returns the target ABI, or else a StringError if the requested ABIName is
// not supported for the given TT and FeatureBits combination.
ABI computeTargetABI(const Triple &TT, FeatureBitset FeatureBits,
                     StringRef ABIName);

ABI getTargetABI(StringRef ABIName);

// Returns the register used to hold the stack pointer after realignment.
MCRegister getBPReg();

// Returns the register holding shadow call stack pointer.
MCRegister getSCSPReg();

} // namespace RISCVABI

namespace RISCVFeatures {

// Validates if the given combination of features are valid for the target
// triple. Exits with report_fatal_error if not.
void validate(const Triple &TT, const FeatureBitset &FeatureBits);

} // namespace RISCVFeatures

enum class RISCVVSEW {
  SEW_8 = 0,
  SEW_16,
  SEW_32,
  SEW_64,
  SEW_128,
  SEW_256,
  SEW_512,
  SEW_1024,
};

enum class RISCVVLMUL {
  LMUL_1 = 0,
  LMUL_2,
  LMUL_4,
  LMUL_8,
  LMUL_RESERVED,
  LMUL_F8,
  LMUL_F4,
  LMUL_F2,
};

namespace RISCVVType {
// Is this a SEW value that can be encoded into the VTYPE format.
inline static bool isValidSEW(unsigned SEW) {
  return isPowerOf2_32(SEW) && SEW >= 8 && SEW <= 1024;
}

// Is this a LMUL value that can be encoded into the VTYPE format.
inline static bool isValidLMUL(unsigned LMUL, bool Fractional) {
  return isPowerOf2_32(LMUL) && LMUL <= 8 && (!Fractional || LMUL != 1);
}

// Encode VTYPE into the binary format used by the the VSETVLI instruction which
// is used by our MC layer representation.
//
// Bits | Name       | Description
// -----+------------+------------------------------------------------
// 7    | vma        | Vector mask agnostic
// 6    | vta        | Vector tail agnostic
// 5:3  | vsew[2:0]  | Standard element width (SEW) setting
// 2:0  | vlmul[2:0] | Vector register group multiplier (LMUL) setting
inline static unsigned encodeVTYPE(RISCVVLMUL VLMUL, RISCVVSEW VSEW,
                                   bool TailAgnostic, bool MaskAgnostic) {
  unsigned VLMULBits = static_cast<unsigned>(VLMUL);
  unsigned VSEWBits = static_cast<unsigned>(VSEW);
  unsigned VTypeI = (VSEWBits << 3) | (VLMULBits & 0x7);
  if (TailAgnostic)
    VTypeI |= 0x40;
  if (MaskAgnostic)
    VTypeI |= 0x80;

  return VTypeI;
}

inline static RISCVVLMUL getVLMUL(unsigned VType) {
  unsigned VLMUL = VType & 0x7;
  return static_cast<RISCVVLMUL>(VLMUL);
}

inline static RISCVVSEW getVSEW(unsigned VType) {
  unsigned VSEW = (VType >> 3) & 0x7;
  return static_cast<RISCVVSEW>(VSEW);
}

inline static bool isTailAgnostic(unsigned VType) { return VType & 0x40; }

inline static bool isMaskAgnostic(unsigned VType) { return VType & 0x80; }

void printVType(unsigned VType, raw_ostream &OS);

} // namespace RISCVVType

namespace RISCVZCE {

enum class SLISTENCODE {
  RA = 0,
  RA_S0,
  RA_S0_S1,
  RA_S0_S2,
  RA_S0_S3,
  RA_S0_S4,
  RA_S0_S5,
  RA_S0_S6,
  RA_S0_S7,
  RA_S0_S8,
  RA_S0_S9,
  RA_S0_S10,
  RA_S0_S11,
  RA_S0_S2_E,
  RA_S0_S3_E,
  RA_S0_S4_E,
};

enum class SLIST16ENCODE {
  RA = 0,
  RA_S0,
  RA_S0_S1,
  RA_S0_S2,
  RA_S0_S3,
  RA_S0_S5 = 6,
  RA_S0_S7 = 8,
  RA_S0_S11 = 12,
};

enum class RLIST3ENCODE {
  NO_MATCH = -1,
  RA = 0,
  RA_S0,
  RA_S0_S1,
  RA_S0_S2,
  RA_S0_S3,
  RA_S0_S5 = 6,
  RA_S0_S7 = 8,
  RA_S0_S11 = 12,
};

enum class RLIST2ENCODE {
  NO_MATCH = -1,
  RA_S0_S2 = 0,
  RA_S0_S3,
  RA_S0_S4,
  RA,
  RA_S0,
  RA_S0_S1,
};

enum class SPIMMINST {
  PUSH,
  PUSH_E,
  POP,
  POP_E,
  POPRET,
  POPRET_E,
  C_POPRET,
  C_POP,
  C_PUSH,
  C_POPRET_E,
  C_POP_E,
  C_PUSH_E,
};

enum class BINARYVALUE_OF_SPIMM {
  B000 = 0,
  B001,
  B010,
  B011,
  B100,
  B101,
};

inline static RLIST2ENCODE encodeRlist2(MCRegister EndReg) {
  switch (EndReg) {
  default:
    llvm_unreachable("Unexpected register");
  case RISCV::X18:
    return RLIST2ENCODE::RA_S0_S2;
  case RISCV::X19:
    return RLIST2ENCODE::RA_S0_S3;
  case RISCV::X20:
    return RLIST2ENCODE::RA_S0_S4;
  case RISCV::X1:
    return RLIST2ENCODE::RA;
  case RISCV::X8:
    return RLIST2ENCODE::RA_S0;
  case RISCV::X9:
    return RLIST2ENCODE::RA_S0_S1;
  }
}

inline static RLIST3ENCODE encodeRlist3(MCRegister EndReg) {
  switch (EndReg) {
  default:
    llvm_unreachable("Unexpected register");
  case RISCV::X1:
    return RLIST3ENCODE::RA;
  case RISCV::X8:
    return RLIST3ENCODE::RA_S0;
  case RISCV::X9:
    return RLIST3ENCODE::RA_S0_S1;
  case RISCV::X18:
    return RLIST3ENCODE::RA_S0_S2;
  case RISCV::X19:
    return RLIST3ENCODE::RA_S0_S3;
  case RISCV::X21:
    return RLIST3ENCODE::RA_S0_S5;
  case RISCV::X23:
    return RLIST3ENCODE::RA_S0_S7;
  case RISCV::X27:
    return RLIST3ENCODE::RA_S0_S11;
  }
}

inline unsigned encodeSlist(MCRegister EndReg, bool isCInst , bool IsRV32E = false) {
  // process the Instructions with "c." and ".e"
  // c.pop/c.push/c.popret[.e]
  if(isCInst){
    if (IsRV32E)
      return static_cast<unsigned>(encodeRlist2(EndReg));
    else
      return static_cast<unsigned>(encodeRlist3(EndReg));
  }

  auto SlistEncode = [=] {
    // process the Instructions with out "c."
    // pop.e/push.e/popret.e
    if (IsRV32E) {
      switch (EndReg) {
      case RISCV::X18:
        return SLISTENCODE::RA_S0_S2_E;
      case RISCV::X19:
        return SLISTENCODE::RA_S0_S3_E;
      case RISCV::X20:
        return SLISTENCODE::RA_S0_S4_E;
      // default:
        // llvm_unreachable("Unexpected register");
      }
    }

    // process the Instructions with out "c." and ".e"
    // pop/push/popret
    switch (EndReg) {
    case RISCV::X1:
      return SLISTENCODE::RA;
    case RISCV::X8:
      return SLISTENCODE::RA_S0;
    case RISCV::X9:
      return SLISTENCODE::RA_S0_S1;
    case RISCV::X18:
      return SLISTENCODE::RA_S0_S2;
    case RISCV::X19:
      return SLISTENCODE::RA_S0_S3;
    case RISCV::X20:
      return SLISTENCODE::RA_S0_S4;
    case RISCV::X21:
      return SLISTENCODE::RA_S0_S5;
    case RISCV::X22:
      return SLISTENCODE::RA_S0_S6;
    case RISCV::X23:
      return SLISTENCODE::RA_S0_S7;
    case RISCV::X24:
      return SLISTENCODE::RA_S0_S8;
    case RISCV::X25:
      return SLISTENCODE::RA_S0_S9;
    case RISCV::X26:
      return SLISTENCODE::RA_S0_S10;
    case RISCV::X27:
      return SLISTENCODE::RA_S0_S11;
    default:
      llvm_unreachable("Unexpected register");
    }
  }();
  return static_cast<unsigned int>(SlistEncode);
}

inline static bool isValidAlist(MCRegister EndReg, unsigned SlistEncode, bool isCInst, bool isRV32E = false) {
  // process the Instructions with out c.
  // pop/push/popret[.e]
  if (EndReg == RISCV::NoRegister && !isCInst)
    return true;
  if (!isCInst){
    switch (static_cast<SLISTENCODE>(SlistEncode)) {
    case SLISTENCODE::RA:
      return EndReg == RISCV::NoRegister;
    case SLISTENCODE::RA_S0:
      return EndReg == RISCV::X10;
    case SLISTENCODE::RA_S0_S1:
      return EndReg == RISCV::X11;
    case SLISTENCODE::RA_S0_S2:
    case SLISTENCODE::RA_S0_S2_E:
      return EndReg == RISCV::X12;
    case SLISTENCODE::RA_S0_S3:
    case SLISTENCODE::RA_S0_S4:
    case SLISTENCODE::RA_S0_S5:
    case SLISTENCODE::RA_S0_S6:
    case SLISTENCODE::RA_S0_S7:
    case SLISTENCODE::RA_S0_S8:
    case SLISTENCODE::RA_S0_S9:
    case SLISTENCODE::RA_S0_S10:
    case SLISTENCODE::RA_S0_S11:
    case SLISTENCODE::RA_S0_S3_E:
    case SLISTENCODE::RA_S0_S4_E:
      return EndReg == RISCV::X13;
    default:
      llvm_unreachable("Unexpected slist encode!");
    }
  }

  // process the Instructions with "c."
  // c.pop/c.push/c.popret
  if (!isRV32E) {
    switch (static_cast<RLIST3ENCODE>(SlistEncode)) {
    case RLIST3ENCODE::RA:
      return EndReg == RISCV::NoRegister;
    case RLIST3ENCODE::RA_S0:
      return EndReg == RISCV::X10;
    case RLIST3ENCODE::RA_S0_S1:
      return EndReg == RISCV::X11;
    case RLIST3ENCODE::RA_S0_S2:
      return EndReg == RISCV::X12;
    case RLIST3ENCODE::RA_S0_S3:
    case RLIST3ENCODE::RA_S0_S5:
    case RLIST3ENCODE::RA_S0_S7:
    case RLIST3ENCODE::RA_S0_S11:
      return EndReg == RISCV::X13;
    default:
      llvm_unreachable("Unexpected slist encode!");
    }
  }

  // process the Instructions with "c." and ".e"
  // c.pop.e/c.push.e/c.popret.e
  switch (static_cast<RLIST2ENCODE>(SlistEncode)) {
    case RLIST2ENCODE::RA_S0_S2:
      return EndReg == RISCV::X12;
    case RLIST2ENCODE::RA_S0_S3:
    case RLIST2ENCODE::RA_S0_S4:
      return EndReg == RISCV::X13;
    case RLIST2ENCODE::RA:
      return EndReg == RISCV::NoRegister;
    case RLIST2ENCODE::RA_S0:
      return EndReg == RISCV::X10;
    case RLIST2ENCODE::RA_S0_S1:
      return EndReg == RISCV::X11;
    default:
      llvm_unreachable("Unexpected slist encode!");
    }
}

#define ENCODE_SPIMM(MAX_SPIMM, VAL, VAL64)                                    \
  spimmVal = (stackAdjustment - (isRV64 ? VAL64 : VAL)) / 16;                  \
  if ((isRV64 && VAL64 == 0) || (!isRV64 && VAL == 0) || spimmVal > MAX_SPIMM) \
    return false;

inline static bool getSpimm(SPIMMINST Inst, unsigned rlistVal,
                            unsigned &spimmVal, int64_t stackAdjustment,
                            bool isRV64) {
  if (Inst >= SPIMMINST::C_POPRET_E) { // use rlist
    RLIST2ENCODE rlist = (RLIST2ENCODE)rlistVal;
    if (Inst == SPIMMINST::C_PUSH_E || Inst == SPIMMINST::C_POPRET_E) {
      switch (rlist) {
      case RLIST2ENCODE::RA_S0_S2:
        ENCODE_SPIMM(5, 16, 0)
        return true;
      case RLIST2ENCODE::RA_S0_S3:
      case RLIST2ENCODE::RA_S0_S4:
        ENCODE_SPIMM(5, 32, 0)
        return true;
      case RLIST2ENCODE::RA:
      case RLIST2ENCODE::RA_S0:
      case RLIST2ENCODE::RA_S0_S1:
        ENCODE_SPIMM(5, 16, 0)
        return true;
      default:
        return false;
      }
    } else if (Inst == SPIMMINST::C_POP_E) {
      switch (rlist) {
      case RLIST2ENCODE::RA_S0_S2:
        ENCODE_SPIMM(5, 16, 0)
        return true;
      case RLIST2ENCODE::RA_S0_S3:
        ENCODE_SPIMM(5, 32, 0)
        return true;
      case RLIST2ENCODE::RA:
      case RLIST2ENCODE::RA_S0:
      case RLIST2ENCODE::RA_S0_S1:
        ENCODE_SPIMM(1, 16, 0)
        return true;
      default:
        return false;
      }
    }
  }
  RLIST3ENCODE rlist = (RLIST3ENCODE)rlistVal;
  SLISTENCODE slist = (SLISTENCODE)rlistVal;
  if (Inst == SPIMMINST::PUSH || Inst == SPIMMINST::PUSH_E ||
      Inst == SPIMMINST::POP || Inst == SPIMMINST::POP_E ||
      Inst == SPIMMINST::POPRET || Inst == SPIMMINST::POPRET_E) {
    switch (slist) {
    case SLISTENCODE::RA:
    case SLISTENCODE::RA_S0:
      ENCODE_SPIMM(31, 16, 16)
      return true;
    case SLISTENCODE::RA_S0_S1:
    case SLISTENCODE::RA_S0_S2:
    case SLISTENCODE::RA_S0_S2_E:
      ENCODE_SPIMM(31, 16, 32)
      return true;
    case SLISTENCODE::RA_S0_S3:
    case SLISTENCODE::RA_S0_S4:
    case SLISTENCODE::RA_S0_S3_E:
    case SLISTENCODE::RA_S0_S4_E:
      ENCODE_SPIMM(31, 32, 48)
      return true;
    case SLISTENCODE::RA_S0_S5:
    case SLISTENCODE::RA_S0_S6:
      ENCODE_SPIMM(31, 32, 64)
      return true;
    case SLISTENCODE::RA_S0_S7:
    case SLISTENCODE::RA_S0_S8:
      ENCODE_SPIMM(31, 48, 80)
      return true;
    case SLISTENCODE::RA_S0_S9:
    case SLISTENCODE::RA_S0_S10:
      ENCODE_SPIMM(31, 48, 96)
      return true;
    case SLISTENCODE::RA_S0_S11:
      ENCODE_SPIMM(31, 64, 112)
      return true;
    default:
      return false;
    }
  } else if (Inst == SPIMMINST::C_POPRET) {
    switch (rlist) {
    case RLIST3ENCODE::RA:
    case RLIST3ENCODE::RA_S0:
      ENCODE_SPIMM(5, 16, 16)
      return true;
    case RLIST3ENCODE::RA_S0_S1:
    case RLIST3ENCODE::RA_S0_S2:
      ENCODE_SPIMM(5, 16, 32)
      return true;
    case RLIST3ENCODE::RA_S0_S3:
      ENCODE_SPIMM(5, 32, 48)
      return true;
    case RLIST3ENCODE::RA_S0_S5:
      ENCODE_SPIMM(5, 32, 64)
      return true;
    case RLIST3ENCODE::RA_S0_S7:
      ENCODE_SPIMM(5, 48, 80)
      return true;
    case RLIST3ENCODE::RA_S0_S11:
      ENCODE_SPIMM(5, 64, 112)
      return true;
    default:
      return false;
    }
  } else if (Inst == SPIMMINST::C_PUSH) {
    switch (rlist) {
    case RLIST3ENCODE::RA:
    case RLIST3ENCODE::RA_S0:
      ENCODE_SPIMM(5, 16, 16)
      return true;
    case RLIST3ENCODE::RA_S0_S1:
    case RLIST3ENCODE::RA_S0_S2:
      ENCODE_SPIMM(5, 16, 32)
      return true;
    case RLIST3ENCODE::RA_S0_S3:
      ENCODE_SPIMM(5, 32, 48)
      return true;
    case RLIST3ENCODE::RA_S0_S5:
      ENCODE_SPIMM(5, 32, 64)
      return true;
    case RLIST3ENCODE::RA_S0_S7:
      ENCODE_SPIMM(5, 48, 80)
      return true;
    case RLIST3ENCODE::RA_S0_S11:
      ENCODE_SPIMM(5, 64, 112)
      return true;
    default:
      return false;
    }
  } else if (Inst == SPIMMINST::C_POP) {
    switch (rlist) {
    case RLIST3ENCODE::RA:
    case RLIST3ENCODE::RA_S0:
      ENCODE_SPIMM(1, 16, 16)
      return true;
    case RLIST3ENCODE::RA_S0_S1:
    case RLIST3ENCODE::RA_S0_S2:
      ENCODE_SPIMM(1, 16, 32)
      return true;
    case RLIST3ENCODE::RA_S0_S3:
      ENCODE_SPIMM(1, 32, 48)
      return true;
    case RLIST3ENCODE::RA_S0_S5:
      ENCODE_SPIMM(1, 32, 64)
      return true;
    case RLIST3ENCODE::RA_S0_S7:
      ENCODE_SPIMM(1, 48, 80)
      return true;
    case RLIST3ENCODE::RA_S0_S11:
      ENCODE_SPIMM(1, 64, 112)
      return true;
    default:
      return false;
    }
  } else {
    return false;
  }
}

inline static unsigned encodeAlist(MCRegister EndReg, unsigned SlistEncode) {
  return (SlistEncode != 0 && EndReg == RISCV::NoRegister) ? 0 : 1;
}

inline static unsigned encodeRetval(int Retval) {
  return Retval == -1 ? 3 : (Retval + 1);
}

inline static RISCVZCE::RLIST3ENCODE
convertRlist2ToRlist3(RISCVZCE::RLIST2ENCODE rlist2) {
  switch ((RISCVZCE::RLIST2ENCODE)rlist2) {
  case RISCVZCE::RLIST2ENCODE::RA:
    return RISCVZCE::RLIST3ENCODE::RA;
  case RISCVZCE::RLIST2ENCODE::RA_S0:
    return RISCVZCE::RLIST3ENCODE::RA_S0;
  case RISCVZCE::RLIST2ENCODE::RA_S0_S1:
    return RISCVZCE::RLIST3ENCODE::RA_S0_S1;
  default:
    return RISCVZCE::RLIST3ENCODE::NO_MATCH;
  }
}

void printAlist(unsigned opcode, unsigned SlistEncode, unsigned areg, raw_ostream &OS);
void printSlist(unsigned SlistEncode, raw_ostream &OS);
void printRetval(unsigned RetvalEncode, raw_ostream &OS);
void printRlist3(unsigned RlistEncode, raw_ostream &OS);
void printRlist2(unsigned RlistEncode, raw_ostream &OS);
void printSpimm(int64_t Spimm, raw_ostream &OS);

} // namespace RISCVZCE

} // namespace llvm

#endif
