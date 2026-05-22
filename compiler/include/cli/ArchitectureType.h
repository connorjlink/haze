#ifndef HAZE_ARCHITECTURETYPE_H
#define HAZE_ARCHITECTURETYPE_H

#include <error/ErrorReporter.h>
#include <x86/X86Register.h>
#include <riscv/RISCVRegister.h>
#include <utility/Constants.h>

// Haze ArchitectureKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ArchitectureKind
	{
#define X(enumerator, name) enumerator,
#include <cli/ArchitectureKind.def>
#undef X
	};

	constexpr std::string_view to_string(ArchitectureKind type)
	{
		switch (type)
		{
#define X(enumerator, name) case ArchitectureKind::enumerator: return name;
#include <cli/ArchitectureKind.def>
#undef X
		}

		return "<unknown architecture kind>";
	}

	constexpr std::pair<std::size_t, std::size_t> get_register_extrema(ArchitectureKind type)
	{
		using enum ArchitectureKind;
		switch (type)
		{
			case X86: return std::make_pair(0, 7); // eax, ..., edi
			case RISCV: return std::make_pair(1, 31); // x1, ..., x31
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"unknown architecture type `{}`", static_cast<int>(type)), NULL_TOKEN);
	}

#define NAMEOF(x) #x

	constexpr const std::string& get_stack_frame_pointer(ArchitectureKind type)
	{
		using enum ArchitectureKind;
		switch (type)
		{
			case X86: return NAMEOF(EBP);
			case RISCV: return NAMEOF(FP); 
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"unknown architecture type `{}`", static_cast<int>(type)), NULL_TOKEN);
	}

	constexpr Address get_linker_origin(ArchitectureKind type)
	{
		using enum ArchitectureKind;
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
