#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include <cli/ArchitectureType.h>

// Haze Instruction.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T>
	class Instruction
	{
	public:
		virtual T itype() const = 0;
		virtual ArchitectureType atype() const = 0;
		virtual byterange emit() const = 0;
	};
}

#endif
