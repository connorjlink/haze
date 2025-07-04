import std;

#include <validator/InstructionValidator.h>

// Haze InstructionValidator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	InstructionValidator::InstructionValidator()
	{
		add_test(Test{ "Instruction()", validate_constructor });
		add_test(Test{ ".bytes()", validate_bytes });

		auto tests = test_count();
	}

	bool InstructionValidator::validate_constructor(ErrorFrame parameters)
	{
		return false;
	}

	bool InstructionValidator::validate_bytes(ErrorFrame parameters)
	{
		return false;
	}
}