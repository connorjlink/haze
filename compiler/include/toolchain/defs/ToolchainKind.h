#ifndef HAZE_TOOLCHAINKIND_H
#define HAZE_TOOLCHAINKIND_H

// Haze ToolchainKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ToolchainKind
	{
#define X(enumerator, name, extension) enumerator,
#include <toolchain/defs/ToolchainKind.def>
#undef X
	};

	constexpr std::string_view to_string(ToolchainKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name, extension) case ToolchainKind::enumerator: return name;
#include <toolchain/defs/ToolchainKind.def>
#undef X
		}

		return "<unknown toolchain kind>";
	}
}

#endif 
