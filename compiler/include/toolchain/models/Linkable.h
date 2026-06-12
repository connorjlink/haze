#ifndef HAZE_LINKABLE_H
#define HAZE_LINKABLE_H

#include <utility/Sum.h>

// Haze Linkable.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	FORWARD_DECLARE_SUM(Command)

	struct FunctionSymbol;
	struct IntermediateCommand;

	struct Linkable
	{
	public:
		// a reference to the FunctionSymbol associated with this
		FunctionSymbol* symbol;
		// object code for the function
		std::vector<CommandHandle> commands;
		// ir code of the function
		std::vector<IntermediateCommand*> ir;
		// entry point of the function 
		Address offset;

	public:
		void optimize(void);

	public:
		Linkable(FunctionSymbol* symbol, std::vector<CommandHandle> commands, std::vector<IntermediateCommand*> ir, Address offset)
			: symbol{ symbol }, commands{ std::move(commands) }, ir{ std::move(ir) }, offset{ offset }
		{
		}
	};
}

#endif
