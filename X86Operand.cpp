import std;

#include "X86Operand.h"

// Haze X86Operand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	X86OperandType X86ImmediateOperand::otype() const
	{
		return X86OperandType::IMMEDIATE;
	}

	X86OperandType X86IndirectOperand::otype() const
	{
		return X86OperandType::INDIRECT;
	}

	X86OperandType X86RegisterOperand::otype() const
	{
		return X86OperandType::REGISTER;
	}

	X86OperandType X86RegisterIndirectOperand::otype() const
	{
		return X86OperandType::REGISTER_INDIRECT;
	}

	X86OperandType X86RegisterDisplacedOperand::otype() const
	{
		return X86OperandType::REGISTER_DISPLACED;
	}
}
