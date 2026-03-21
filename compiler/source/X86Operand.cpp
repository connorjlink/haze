import std;

#include <x86/X86Operand.h>

// Haze X86Operand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz::x86
{
	X86OperandType ImmediateOperand::otype() const
	{
		return X86OperandType::IMMEDIATE;
	}

	X86OperandType IndirectOperand::otype() const
	{
		return X86OperandType::INDIRECT;
	}

	X86OperandType RegisterOperand::otype() const
	{
		return X86OperandType::REGISTER;
	}

	X86OperandType RegisterIndirectOperand::otype() const
	{
		return X86OperandType::REGISTER_INDIRECT;
	}

	X86OperandType RegisterDisplacedOperand::otype() const
	{
		return X86OperandType::REGISTER_DISPLACED;
	}
}
