#ifndef HAZE_LINKABLE_H
#define HAZE_LINKABLE_H

#include "Symbol.h"
#include "InstructionCommand.h"

#include <vector>
#include <cstdint>

namespace hz
{
	class Linkable
	{
	public:
		Symbol* symbol;
		std::vector<InstructionCommand*> object_code;
		std::uint16_t offset;

	public:
		Linkable(Symbol* symbol, std::vector<InstructionCommand*>&& object_code, std::uint16_t offset)
			: symbol{ symbol }, object_code{ std::move(object_code) }, offset{ offset }
		{
		}
	};
}

#endif
