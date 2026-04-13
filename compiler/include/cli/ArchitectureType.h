#ifndef HAZE_ARCHITECTURETYPE_H
#define HAZE_ARCHITECTURETYPE_H

#include <error/ErrorReporter.h>
#include <x86/X86Register.h>
#include <riscv/RISCVRegister.h>
#include <utility/Constants.h>

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

	inline constexpr std::pair<std::size_t, std::size_t> get_register_extrema(ArchitectureType type)
	{
		using enum ArchitectureType;
		switch (type)
		{
			case X86: return std::make_pair(0, 7); // eax, ..., edi
			case RISCV: return std::make_pair(1, 31); // x1, ..., x31
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"unknown architecture type `{}`", static_cast<int>(type)), NULL_TOKEN);
	}

#define NAMEOF(x) #x

	inline constexpr std::string_view get_stack_frame_pointer(ArchitectureType type)
	{
		using enum ArchitectureType;
		switch (type)
		{
			case X86: return NAMEOF(EBP);
			case RISCV: return NAMEOF(FP); 
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"unknown architecture type `{}`", static_cast<int>(type)), NULL_TOKEN);
	}

	inline constexpr Address get_linker_origin(ArchitectureType type)
	{
		using enum ArchitectureType;
		switch (type)
		{
			case X86: return 0x401000; // default image base for PE files
			case RISCV: return 0x10000; // arbitrary choice aligned with RARS
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"unknown architecture type `{}`", static_cast<int>(type)), NULL_TOKEN);
	}

#undef NAMEOF
}

#endif 
