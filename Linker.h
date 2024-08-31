#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include "Linkable.h"
#include "InstructionCommand.h"
#include "ErrorReporter.h"
#include "LinkerType.h"

#include <cstdint>

// Haze Linker.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Linker
	{
	public:
		Linker(const std::string&);
		virtual ~Linker();

	public:
		virtual LinkerType ltype() const = 0;
		virtual bool optimize() = 0;
		virtual std::vector<InstructionCommand*> link(std::uint32_t) = 0;
	};

	extern Linker* _linker;
}

#endif
