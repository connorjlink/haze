#ifndef HAZE_ARCHITECTURETYPE_H
#define HAZE_ARCHITECTURETYPE_H

#include <string>
#include <unordered_map>

namespace hz
{
	enum class ArchitectureType
	{
		HAZE,
		X86,
	};

	static const std::unordered_map<std::string, ArchitectureType> _architecture_map
	{
		{ "haze", ArchitectureType::HAZE },
		{ "x86", ArchitectureType::X86 },
	};
}

#endif 
