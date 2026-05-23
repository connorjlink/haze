#ifndef HAZE_INSTRUCTIONVALIDATOR_H
#define HAZE_INSTRUCTIONVALIDATOR_H

#include <validator/Validator.h>

// Haze InstructionValidator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class InstructionValidator : public Validator
	{
	private:
		struct Parameters
		{
			ErrorFrame error_frame;
		};

	private:
		using Test = Test<Parameters>;

	private:
		static bool validate_constructor(Parameters&);
		static bool validate_bytes(Parameters&);

	public:
		InstructionValidator();
	};
}

#endif
