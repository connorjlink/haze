#ifndef HAZE_COMMANDLINEOPTIONS_H
#define HAZE_COMMANDLINEOPTIONS_H

#include "ArchitectureType.h"
#include "VerbosityType.h"
#include "OptimizationType.h"
#include "OptionType.h"

#include <cstdint>

namespace hz
{
	struct CommandLineOptions
	{
		ArchitectureType _architecture = ArchitectureType::HAZE;
		VerbosityType _verbosity = VerbosityType::NORMAL;
		// bit flags by optimization category of OptimizationType
		std::uint32_t _optimization{};
	};

	extern CommandLineOptions* _options;
}

#endif
