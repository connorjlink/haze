#ifndef HAZE_VERBOSITYTYPE_H
#define HAZE_VERBOSITYTYPE_H

// Haze VerbosityType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class VerbosityType
	{
		QUIET,
		NORMAL,
		VERBOSE,
	};

	static const std::unordered_map<std::string, VerbosityType> _verbosity_type_map
	{
		{ "quiet", VerbosityType::QUIET },
		{ "normal", VerbosityType::NORMAL },
		{ "verbose", VerbosityType::VERBOSE },
	};
}

#endif
