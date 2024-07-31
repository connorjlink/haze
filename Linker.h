#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include "Linkable.h"
#include "InstructionCommand.h"

#include "LinkerType.h"

#include <vector>
#include <cstdint>

namespace hz
{
	class Linker
	{
	public:
		virtual LinkerType ltype() const = 0;
		virtual bool optimize() = 0;
		virtual std::vector<InstructionCommand*> link(std::uint16_t) = 0;
	};
}

#endif
