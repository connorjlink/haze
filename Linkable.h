#ifndef HAZE_LINKABLE_H
#define HAZE_LINKABLE_H

#include "Symbol.h"
#include "InstructionCommand.h"

#include <vector>
#include <cstdint>

// Haze Linkable.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Linkable
	{
	public:
		// a reference to the FunctionSymbol associated with this
		Symbol* symbol;
		// object code for the function
		std::vector<Command*> commands;
		// entry point of the function 
		std::uint32_t offset;

	public:
		Linkable(Symbol* symbol, std::vector<Command*>&& object_code, std::uint32_t offset)
			: symbol{ symbol }, commands{ std::move(object_code) }, offset{ offset }
		{
		}
	};
}

#endif
