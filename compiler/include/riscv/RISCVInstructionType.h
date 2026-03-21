#ifndef HAZE_RISCVINSTRUCTIONTYPE_H
#define HAZE_RISCVINSTRUCTIONTYPE_H

// Haze RISCVInstructionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVInstructionType
	{
		// I instruction set
		LB, LH, LW, LBU, LHU,
		SB, SH, SW,
		LI, LUI, AUIPC,
		ADDI, ADD, SUB,
		SLTI, SLT, SLTIU, SLTU,
		ANDI, AND, ORI, OR, XORI, XOR,
		SLLI, SLL, SRLI, SRL, SRAI, SRA,
		JAL, JALR,
		BEQ, BNE, BLT, BGE, BLTU, BGEU,
		NOP, ECALL, EBREAK, FENCE,

		// M extension
		MUL, MULH, MULHU, MULHSU, DIV, DIVU, REM, REMU,

		// A extension
		LRW, SCW,
		AMOSWAPW, AMOADDW, AMOXORW, AMOANDW, AMOORW, AMOMINW, AMOMAXW, AMOMINUW, AMOMAXUW,

		// C extension
		CLWSP, CSWSP, CLW, CSW,
		CJ, CJAL, CJR, CJALR,
		CBEQZ, CBNEZ,
		CLI, CLUI,
		CADDI, CADDI16SP, CADDI4SPN,
		CSLLI, CSRLI, CSRAI,
		CANDI, CMV, CADD, CAND, COR, CXOR, CSUB,
		CNOP, CEBREAK,

		// Zicsr extension
		CSRRW, CSRRS, CSRRC, CSRRWI, CSRRSI, CSRRCI,

		// Zifencei extension
		FENCEI,

		// U/S mode
		URET, SRET, MRET, WFI,
	};
}

#endif
