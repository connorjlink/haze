#ifndef HAZE_OPTIONTYPE_H
#define HAZE_OPTIONTYPE_H

#include <string>
#include <unordered_map>

namespace hz
{
	enum class OptionType
	{
		ARCHITECTURE,
		VERBOSITY,
		OPTIMIZATION,
	};

	static const std::unordered_map<std::string, OptionType> _option_map
	{
		{ "architecture", OptionType::ARCHITECTURE },
		{ "verbosity", OptionType::VERBOSITY },
		{ "optimization", OptionType::OPTIMIZATION },
	};
}

#endif 
