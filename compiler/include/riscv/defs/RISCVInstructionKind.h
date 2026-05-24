#ifndef HAZE_RISCVINSTRUCTIONKIND_H
#define HAZE_RISCVINSTRUCTIONKIND_H

// Haze RISCVInstructionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVInstructionKind
	{
#define X(enumerator, name) enumerator,
#include <riscv/defs/RISCVInstructionKind.x>
#undef X
	};

	constexpr std::string_view to_string(RISCVInstructionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case RISCVInstructionKind::enumerator: return name;
#include <riscv/defs/RISCVInstructionKind.x>
#undef X
		}

		return "<unknown RISC-V instruction kind>";
	}
}

#endif
