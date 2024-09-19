import std;

#include "InstructionValidator.h"
#include "Log.h"

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

	bool InstructionValidator::validate_constructor(TestParameters parameters)
	{
		return false;
	}

	bool InstructionValidator::validate_bytes(TestParameters parameters)
	{
		return false;
	}
}