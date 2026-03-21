#ifndef HAZE_X86INSTRUCTIONTYPE_H
#define HAZE_X86INSTRUCTIONTYPE_H

// Haze X86InstructionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class X86InstructionType
	{
		PUSH,
		POP,

		MOV,
		MOVZX,

		ADD,
		SUB,
		OR,
		AND,
		XOR,

		INC,
		DEC,

		TEST,
		CMP,

		// jcc
		CALL, APICALL, JMP, JE, JNE, JL, JLE, JG, JGE, JA, JAE, JB, JBE,

		// setcc
		SETE, SETNE, SETL, SETLE, SETG, SETGE, SETA, SETAE, SETB, SETBE,

		RET,
		
		LEAVE,
	};
}

#endif
