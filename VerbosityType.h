#ifndef HAZE_VERBOSITYTYPE_H
#define HAZE_VERBOSITYTYPE_H

#include <string>
#include <unordered_map>

namespace hz
{
	enum class VerbosityType
	{
		QUIET,
		NORMAL,
		VERBOSE,
	};

	static const std::unordered_map<std::string, VerbosityType> _verbosity_map
	{
		{ "quiet", VerbosityType::QUIET },
		{ "normal", VerbosityType::NORMAL },
		{ "verbose", VerbosityType::VERBOSE },
	};
}

#endif
