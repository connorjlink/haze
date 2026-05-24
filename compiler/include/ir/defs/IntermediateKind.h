#ifndef HAZE_INTERMEDIATEKIND_H
#define HAZE_INTERMEDIATEKIND_H

// Haze IntermediateKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class IntermediateKind
	{
#define X(enumerator, name) enumerator,
#include <ir/defs/IntermediateKind.x>
#undef X
	};

	constexpr std::string_view to_string(IntermediateKind kind) noexcept
	{
		switch (kind)
		{
#define X(enumerator, name) case IntermediateKind::enumerator: return #name;
#include <ir/defs/IntermediateKind.x>
#undef X
		}

		return "<unknown intermediate kind>";
	}
}

#endif 
