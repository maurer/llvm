/*===-- llvm-c/MCInst.h - Disassembler Public C Interface ---*- C -*-===*\
|*                                                                            *|
|*                     The LLVM Compiler Infrastructure                       *|
|*                                                                            *|
|* This file is distributed under the University of Illinois Open Source      *|
|* License. See LICENSE.TXT for details.                                      *|
|*                                                                            *|
|*===----------------------------------------------------------------------===*|
|*                                                                            *|
|* This header provides a public interface the MCInst type.                   *|
|* LLVM provides an implementation of this interface.                         *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifndef LLVM_C_MCINST_H
#define LLVM_C_MCINST_H

#include "llvm/Support/DataTypes.h"
#include <stddef.h>

/**
 * @defgroup LLVMCInst MCInst
 * @ingroup LLVMC
 *
 * @{
 */

/**
 * An opaque reference to a native instruction.
 */
typedef void *LLVMMCInstRef;

/**
 * Opaque type for MCOperand.
 */
typedef void* LLVMMCOperandRef;

/**
 * Type tag for different kinds of operands.
 */
typedef enum {
  LLVMOTInvalid,
  LLVMOTReg,
  LLVMOTImm,
  LLVMOTFPImm,
  LLVMOTExpr,
  LLVMOTInst
} LLVMMCOperandType;

/**
 * A native instruction operand, projected to C.
 */
typedef struct {
  LLVMMCOperandType Kind;
  union {
    unsigned RegVal;
    int64_t ImmVal;
    double FPImmVal;
    LLVMMCInstRef InstVal;
  };
} LLVMMCOperand;


#ifdef __cplusplus
extern "C" {
#endif /* !defined(__cplusplus) */

/**
 * Dispose of an instruction.
 */
void LLVMMCInstRefDispose(LLVMMCInstRef Inst);

/**
 * Get total number of operands for an instruction.
 */
unsigned LLVMMCInstGetNumOperands(LLVMMCInstRef Inst);

/**
 * Get an abstract form of the ith operand of Inst.
 * You may want to chain this with LLVMMCOperandProject.
 * Note that the lifetime of this LLVMMCOperandRef is the same as the
 * LLVMMCInstRef you created it from - when you dispose the Inst, the
 * operand becomes invalid as well.
 */
LLVMMCOperandRef LLVMMCInstGetOperand(LLVMMCInstRef Inst, unsigned i);

/**
 * Get the opcode of a particular MCInst.
 * Reference tblgen files for actual values.
 */
unsigned LLVMMCInstGetOpcode(LLVMMCInstRef Inst);

/**
 * Project an MCOperand into a C-struct
 */
void LLVMMCOperandProject(LLVMMCOperandRef OperandCPP, LLVMMCOperand* Operand);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* !defined(__cplusplus) */

#endif /* !defined(LLVM_C_MCINST_H) */
