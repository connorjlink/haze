#ifndef HAZE_COMMANDLINEOPTIONS_H
#define HAZE_COMMANDLINEOPTIONS_H

#include <cli/ArchitectureKind.h>
#include <cli/ExecutionKind.h>
#include <cli/VerbosityKind.h>
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
		// --execution=
		ExecutionKind execution = ExecutionKind::COMPILE;

		// --output=raw
		bool raw_output = false;

		// --outfile=
		std::string output_filepath{};

		// bit flags by optimization category of OptimizationType
		std::uint32_t optimization{};
	};
}

#endif
