#ifndef HAZE_RISCVINSTRUCTIONKIND_H
#define HAZE_RISCVINSTRUCTIONKIND_H

#include <utility/AutoEnum.h>

// Haze RISCVInstructionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define RISCV_INSTRUCTION_KINDS(X) \
	/* I instruction format */ \
	X(LB, lb) \
	X(LH, lh) \
	X(LW, lw) \
	X(LBU, lbu) \
	X(LHU, lhu) \
	X(SB, sb) \
	X(SH, sh) \
	X(SW, sw) \
	X(LI, li) \
	X(LUI, lui) \
	X(AUIPC, auipc) \
	X(ADDI, addi) \
	X(ADD, add) \
	X(SUB, sub) \
	X(SLTI, slti) \
	X(SLT, slt) \
	X(SLTIU, sltiu) \
	X(SLTU, sltu) \
	X(ANDI, andi) \
	X(AND, and) \
	X(ORI, ori) \
	X(OR, or) \
	X(XORI, xori) \
	X(XOR, xor) \
	X(SLLI, slli) \
	X(SLL, sll) \
	X(SRLI, srli) \
	X(SRL, srl) \
	X(SRAI, srai) \
	X(SRA, sra) \
	X(JAL, jal) \
	X(JALR, jalr) \
	X(BEQ, beq) \
	X(BNE, bne) \
	X(BLT, blt) \
	X(BGE, bge) \
	X(BLTU, bltu) \
	X(BGEU, bgeu) \
	X(NOP, nop) \
	X(ECALL, ecall) \
	X(EBREAK, ebreak) \
	X(FENCE, fence) \
	/* M extension */ \
	X(MUL, mul) \
	X(MULH, mulh) \
	X(MULHU, mulhu) \
	X(MULHSU, mulhsu) \
	X(DIV, div) \
	X(DIVU, divu) \
	X(REM, rem) \
	X(REMU, remu) \
	/* A extension */ \
	X(LRW, lr.w) \
	X(SCW, sc.w) \
	X(AMOSWAPW, amoswap.w) \
	X(AMOADDW, amoadd.w) \
	X(AMOXORW, amoxor.w) \
	X(AMOANDW, amoand.w) \
	X(AMOORW, amoor.w) \
	X(AMOMINW, amomin.w) \
	X(AMOMAXW, amomax.w) \
	X(AMOMINUW, amominu.w) \
	X(AMOMAXUW, amomaxu.w) \
	/* C extension */ \
	X(CLWSP, c.lwsp) \
	X(CSWSP, c.swsp) \
	X(CLW, c.lw) \
	X(CSW, c.sw) \
	X(CJ, c.j) \
	X(CJAL, c.jal) \
	X(CJR, c.jr) \
	X(CJALR, c.jalr) \
	X(CBEQZ, c.beqz) \
	X(CBNEZ, c.bnez) \
	X(CLI, c.li) \
	X(CLUI, c.lui) \
	X(CADDI, c.addi) \
	X(CADDI16SP, c.addi16sp) \
	X(CADDI4SPN, c.addi4spn) \
	X(CSLLI, c.slli) \
	X(CSRLI, c.srli) \
	X(CSRAI, c.srai) \
	X(CANDI, c.andi) \
	X(CMV, c.mv) \
	X(CADD, c.add) \
	X(CAND, c.and) \
	X(COR, c.or) \
	X(CXOR, c.xor) \
	X(CSUB, c.sub) \
	X(CNOP, c.nop) \
	X(CEBREAK, c.ebreak) \
	/* Zicsr extension */ \
	X(CSRRW, csrrw) \
	X(CSRRS, csrrs) \
	X(CSRRC, csrrc) \
	X(CSRRWI, csrrwi) \
	X(CSRRSI, csrrsi) \
	X(CSRRCI, csrci) \
	/* Zifencei extension */ \
	X(FENCEI, fence.i) \
	/* U/S mode */ \
	X(URET, uret) \
	X(SRET, sret) \
	X(MRET, mret) \
	X(WFI, wfi)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case RISCVInstructionKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, RISCV_INSTRUCTION_KINDS, RISCVInstructionKind, RISC-V instruction kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
