//===- lib/MC/MCInst.cpp - MCInst implementation --------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm-c/MCInst.h"

using namespace llvm;

void MCOperand::print(raw_ostream &OS, const MCAsmInfo *MAI) const {
  OS << "<MCOperand ";
  if (!isValid())
    OS << "INVALID";
  else if (isReg())
    OS << "Reg:" << getReg();
  else if (isImm())
    OS << "Imm:" << getImm();
  else if (isExpr()) {
    OS << "Expr:(" << *getExpr() << ")";
  } else if (isInst()) {
    OS << "Inst:(" << *getInst() << ")";
  } else
    OS << "UNDEFINED";
  OS << ">";
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
void MCOperand::dump() const {
  print(dbgs(), nullptr);
  dbgs() << "\n";
}
#endif

void MCInst::print(raw_ostream &OS, const MCAsmInfo *MAI) const {
  OS << "<MCInst " << getOpcode();
  for (unsigned i = 0, e = getNumOperands(); i != e; ++i) {
    OS << " ";
    getOperand(i).print(OS, MAI);
  }
  OS << ">";
}

void MCInst::dump_pretty(raw_ostream &OS, const MCAsmInfo *MAI,
                         const MCInstPrinter *Printer,
                         StringRef Separator) const {
  OS << "<MCInst #" << getOpcode();

  // Show the instruction opcode name if we have access to a printer.
  if (Printer)
    OS << ' ' << Printer->getOpcodeName(getOpcode());

  for (unsigned i = 0, e = getNumOperands(); i != e; ++i) {
    OS << Separator;
    getOperand(i).print(OS, MAI);
  }
  OS << ">";
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
void MCInst::dump() const {
  print(dbgs(), nullptr);
  dbgs() << "\n";
}
#endif

void LLVMMCInstRefDispose(LLVMMCInstRef InstR) {
  MCInst* Inst = (MCInst*)InstR;
  delete Inst;
}

unsigned LLVMMCInstGetNumOperands(LLVMMCInstRef InstR) {
  MCInst* Inst = (MCInst*)InstR;
  return Inst->getNumOperands();
}

LLVMMCOperandRef LLVMMCInstGetOperand(LLVMMCInstRef InstR, unsigned i) {
  MCInst* Inst = (MCInst*)InstR;
  return &(Inst->getOperand(i));
}

unsigned LLVMMCInstGetOpcode(LLVMMCInstRef InstR) {
  MCInst* Inst = (MCInst*)InstR;
  return Inst->getOpcode();
}

void LLVMMCOperandProject(LLVMMCOperandRef OperandR, LLVMMCOperand* OperandC) {
  MCOperand* OperandCPP = (MCOperand*)OperandR;
  if (OperandCPP->isReg()) {
    OperandC->Kind     = LLVMOTReg;
    OperandC->RegVal   = OperandCPP->getReg();
  } else if (OperandCPP->isImm()) {
    OperandC->Kind     = LLVMOTImm;
    OperandC->ImmVal   = OperandCPP->getImm();
  } else if (OperandCPP->isReg()) {
    OperandC->Kind     = LLVMOTFPImm;
    OperandC->FPImmVal = OperandCPP->getFPImm();
  } else if (OperandCPP->isExpr()) {
    OperandC->Kind     = LLVMOTExpr;
    //XXX: Haven't modeled Expr yet as this is for disasm
  } else if (OperandCPP->isInst()) {
    OperandC->Kind     = LLVMOTInst;
    OperandC->InstVal  = (LLVMMCInstRef)OperandCPP->getInst();
  } else {
    assert((!OperandCPP.isValid()) && "Unknown Operand Type");
    OperandC->Kind     = LLVMOTInvalid;
  }
}
