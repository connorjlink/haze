#ifndef HAZE_COMMANDLINEOPTIONS_H
#define HAZE_COMMANDLINEOPTIONS_H

#include <cli/defs/ArchitectureKind.h>
#include <cli/defs/OptimizationFlag.h>
#include <cli/defs/VerbosityKind.h>
#include <data/DependencyInjector.h>

// Haze CommandLineOptions.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct CommandLineOptions 
		: public SingletonTag<CommandLineOptions>
	{
		// --architecture=
		ArchitectureKind architecture = ArchitectureKind::X86;
		// --verbosity=
		VerbosityKind verbosity = VerbosityKind::NORMAL;

		// --output=raw
		bool raw_output = false;

		// --outfile=
		std::string output_filepath{};

		// bit flags by optimization category of OptimizationType
		OptimizationFlag optimization{};
	};
}

#endif
