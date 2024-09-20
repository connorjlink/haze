#ifndef HAZE_LINKABLE_H
#define HAZE_LINKABLE_H

#include "Symbol.h"
#include "Command.h"
#include "IntermediateCommand.h"

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
		// ir code of the function
		std::vector<IntermediateCommand*> ir;
		// entry point of the function 
		std::uint32_t offset;

	public:
		Linkable(Symbol* symbol, std::vector<Command*>&& commands, std::vector<IntermediateCommand*>&& ir, std::uint32_t offset)
			: symbol{ symbol }, commands{ std::move(commands) }, ir{ std::move(ir) }, offset{ offset }
		{
		}
	};
}

#endif
