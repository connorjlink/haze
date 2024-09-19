#ifndef HAZE_ARCHITECTURETYPE_H
#define HAZE_ARCHITECTURETYPE_H

// Haze ArchitectureType.h
// (c) Connor J. Link. All Rights Reserved.

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
