#ifndef HAZE_COMMANDLINEOPTIONS_H
#define HAZE_COMMANDLINEOPTIONS_H

#include "ArchitectureType.h"
#include "VerbosityType.h"
#include "ExecutionType.h"
#include "OptimizationType.h"
#include "DependencyInjector.h"
#include "OptionType.h"

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
