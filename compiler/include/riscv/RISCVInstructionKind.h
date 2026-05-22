#ifndef HAZE_RISCVINSTRUCTIONKIND_H
#define HAZE_RISCVINSTRUCTIONKIND_H

// Haze RISCVInstructionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVInstructionKind
	{
#define X(enumerator, name) enumerator,
#include <riscv/RISCVInstructionKind.def>
#undef X
	};

	constexpr std::string_view to_string(RISCVInstructionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case RISCVInstructionKind::enumerator: return name;
#include <riscv/RISCVInstructionKind.def>
#undef X
		}

		return "<unknown RISC-V instruction kind>";
	}
}

template<>
struct std::formatter<hz::RISCVInstructionKind>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}
	auto format(const hz::RISCVInstructionKind& kind, std::format_context& context) const
	{
		return std::format_to(context.out(), "{}", to_string(kind));
	}
};

#endif
