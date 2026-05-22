#ifndef HAZE_COMMANDLINEOPTIONS_H
#define HAZE_COMMANDLINEOPTIONS_H

#include <cli/ArchitectureKind.h>
#include <cli/ExecutionType.h>
#include <cli/VerbosityType.h>
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
		VerbosityType verbosity = VerbosityType::NORMAL;
		// --execution=
		ExecutionType execution = ExecutionType::COMPILE;

		// --output=raw
		bool raw_output = false;

		// --outfile=
		std::string output_filepath{};

		// bit flags by optimization category of OptimizationType
		std::uint32_t optimization{};
	};
}

#endif
