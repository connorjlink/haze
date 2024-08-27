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
		EXECUTION,
		OPTIMIZATION,
	};

	static const std::unordered_map<std::string, OptionType> _option_map
	{
		{ "architecture", OptionType::ARCHITECTURE },
		{ "verbosity", OptionType::VERBOSITY },
		{ "execution", OptionType::EXECUTION },
		{ "optimization", OptionType::OPTIMIZATION },
	};
}

#endif 