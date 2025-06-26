#ifndef HAZE_INSTRUCTIONVALIDATOR_H
#define HAZE_INSTRUCTIONVALIDATOR_H

#include "Validator.h"

// Haze InstructionValidator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class InstructionValidator : public Validator
	{
	private:
		static bool validate_constructor(ErrorFrame);
		static bool validate_bytes(ErrorFrame);

	public:
		InstructionValidator();
	};
}

#endif
