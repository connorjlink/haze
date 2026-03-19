#ifndef HAZE_RISCVARITHMETICINSTRUCTIONTYPE_H
#define HAZE_RISCVARITHMETICINSTRUCTIONTYPE_H

// Haze RISCVArithmeticInstructionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVArithmeticInstructionType
	{
		ADD, SUB,
		AND, OR, XOR,
		SLL, SRL, SRA,
	};
}

#endif
