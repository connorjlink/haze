#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include <command/InstructionCommand.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/LinkerType.h>
#include <utility/PlatformVariables.h>

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
		virtual std::vector<InstructionCommand*> link(native_uint, native_uint) = 0;
	};
}

#endif
