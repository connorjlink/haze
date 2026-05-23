import std;

#include <validator/InstructionValidator.h>

// Haze InstructionValidator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	InstructionValidator::InstructionValidator()
	{
#pragma message("TODO: implement instruction validator tests")
		add_test(Test{ "Instruction()", validate_constructor });
		add_test(Test{ ".bytes()", validate_bytes });

		auto tests = test_count();
	}

	bool InstructionValidator::validate_constructor(Parameters& parameters)
	{
		return false;
	}

	bool InstructionValidator::validate_bytes(Parameters& parameters)
	{
		return false;
	}
}