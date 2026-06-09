#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include <cli/CommandLineOptions.h>
#include <cli/defs/ArchitectureKind.h>
#include <command/Command.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/models/Linkable.h>

// Haze Linker.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// Has to be a singleton because many threads will feed compiled 
	// translation units to the linker to produce a final command stream
	struct Linker
		: public SingletonTag<Linker>
		, public InjectSingleton<ErrorReporter, SymbolDatabase, CommandLineOptions>
	{
	private:
		// mapping function name (global scope, shared across files) -> linkable
		std::unordered_map<std::string, Linkable> linkables;

	public:
		virtual ~Linker() = default;

	public:
		void reload(const std::vector<Linkable>&);
		// NOTE: the linker does not support generating fat binaries, so a single architecture is sufficient to uniquely identify it
		ArchitectureKind architecture_kind() const;
		bool optimize();
		std::vector<CommandReference<InstructionCommand>> link(Address, Address);
	};
}

#endif
