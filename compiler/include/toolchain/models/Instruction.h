#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include <cli/ArchitectureKind.h>
#include <utility/Constants.h>

// Haze Instruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Instruction
	{
	public:
		virtual ArchitectureKind ctype() const = 0;
		virtual ByteRange emit() const = 0;
	};
}

#endif
