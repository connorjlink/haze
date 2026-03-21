#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include <command/InstructionCommand.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/LinkerType.h>
#include <toolchain/models/Linkable.h>
#include <utility/PlatformVariables.h>
#include <cli/CommandLineOptions.h>

// Haze Linker.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// Has to be a singleton because many threads will feed compiled 
	// translation units to the linker to produce a final command stream
	class Linker
		: public SingletonTag<Linker>
		, public InjectSingleton<ErrorReporter, SymbolDatabase, CommandLineOptions>
	{
	private:
		// mapping function name (global scope, shared across files) -> linkable
		std::unordered_map<std::string, Linkable> _linkables;

	public:
		// assembler linker constructor
		Linker(std::vector<InstructionCommand*>&&, const std::string&);
		// compiler linker constructor
		Linker(std::vector<Linkable>&&, const std::string&);
		~Linker();

	public:
		LinkerType ltype() const;
		bool optimize();
		std::vector<InstructionCommand*> link(native_uint, native_uint);
	};
}

#endif
