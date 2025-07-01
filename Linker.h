#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include "Linkable.h"
#include "InstructionCommand.h"
#include "ErrorReporter.h"
#include "LinkerType.h"
#include "SymbolDatabase.h"
#include "DependencyInjector.h"

// Haze Linker.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// Has to be a singleton because many threads will feed compiled 
	// translation units to the linker to produce a final command stream
	class Linker
		: public SingletonTag<Linker>
		, public InjectSingleton<ErrorReporter, SymbolDatabase>
	{
	public:
		Linker(const std::string&);
		virtual ~Linker();

	public:
		virtual LinkerType ltype() const = 0;
		virtual bool optimize() = 0;
		virtual std::vector<InstructionCommand*> link(std::uint32_t) = 0;
	};
}

#endif
