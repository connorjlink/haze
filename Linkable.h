#ifndef HAZE_LINKABLE_H
#define HAZE_LINKABLE_H

#include "Symbol.h"
#include "Instruction.h"

#include <vector>

namespace hz
{
	class Linkable
	{
	public:
		Symbol* symbol;
		std::vector<Instruction*> object_code;
		std::size_t offset;

	public:
		Linkable(Symbol* symbol, std::vector<Instruction>&& object_code, std::size_t offset)
			: symbol{ symbol }, object_code{ std::move(object_code) }, offset{ offset }
		{
		}
	};
}

#endif
