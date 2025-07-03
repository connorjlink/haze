#ifndef HAZE_OPTIONTYPE_H
#define HAZE_OPTIONTYPE_H

// Haze OptionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class OptionType
	{
		ARCHITECTURE,
		VERBOSITY,
		EXECUTION,
		OPTIMIZATION,
		OUTPUT
	};

	static const std::unordered_map<std::string_view, OptionType> _option_map
	{
		{ "architecture", OptionType::ARCHITECTURE },
		{ "verbosity", OptionType::VERBOSITY },
		{ "execution", OptionType::EXECUTION },
		{ "output", OptionType::OUTPUT },
		{ "optimization", OptionType::OPTIMIZATION },
	};
}

#endif 
