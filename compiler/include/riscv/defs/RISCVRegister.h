#ifndef HAZE_RISCVREGISTER_H
#define HAZE_RISCVREGISTER_H

#include <utility/Constants.h>

// Haze RISCVRegister.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVRegister : Register
	{
#define X(enumerator, name, value) enumerator = value,
#include <riscv/defs/RISCVRegister.def>
#undef X
	};

	constexpr std::string_view to_string(RISCVRegister reg)
	{
		switch (reg)
		{
#define X(enumerator, name, value) case RISCVRegister::enumerator: return name;
#include <riscv/defs/RISCVRegister.def>
#undef X
		}

		return "<unknown RISC-V register>";
	}
}

#endif
