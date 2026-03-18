#ifndef HAZE_ARCHITECTURETYPE_H
#define HAZE_ARCHITECTURETYPE_H

// Haze ArchitectureType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ArchitectureType
	{
		X86,
		RISCV,
	};

	static const std::unordered_map<std::string, ArchitectureType> _architecture_map
	{
		{ "x86", ArchitectureType::X86 },
		{ "riscv", ArchitectureType::RISCV },
	};
}

#endif 
