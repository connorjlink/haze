#ifndef HAZE_X86JCCINSTRUCTIONTYPE_H
#define HAZE_X86JCCINSTRUCTIONTYPE_H

// Haze X86JccInstructionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class X86JccInstructionType
	{
		API_CALL,
		CALL,
		JMP,
		JE,
		JNE,
		JL,
		JLE,
		JG,
		JGE,
		JA,
		JAE,
		JB,
		JBE,
	};
}

#endif 
