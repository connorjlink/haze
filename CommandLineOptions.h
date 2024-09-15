#ifndef HAZE_COMMANDLINEOPTIONS_H
#define HAZE_COMMANDLINEOPTIONS_H

#include "ArchitectureType.h"
#include "VerbosityType.h"
#include "ExecutionType.h"
#include "OptimizationType.h"
#include "OptionType.h"

import std;

// Haze CommandLineOptions.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct CommandLineOptions
	{
		ArchitectureType _architecture = ArchitectureType::HAZE;
		VerbosityType _verbosity = VerbosityType::NORMAL;
		ExecutionType _execution = ExecutionType::COMPILE;
		// bit flags by optimization category of OptimizationType
		std::uint32_t _optimization{};
	};

	extern CommandLineOptions* _options;
}

#endif
