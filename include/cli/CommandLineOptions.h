#ifndef HAZE_COMMANDLINEOPTIONS_H
#define HAZE_COMMANDLINEOPTIONS_H

#include <cli/ArchitectureType.h>
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
		ArchitectureType _architecture = ArchitectureType::HAZE;
		// --verbosity=
		VerbosityType _verbosity = VerbosityType::NORMAL;
		// --execution=
		ExecutionType _execution = ExecutionType::COMPILE;

		// --output=raw
		bool _raw_output = false;

		// bit flags by optimization category of OptimizationType
		std::uint32_t _optimization{};
	};
}

#endif
