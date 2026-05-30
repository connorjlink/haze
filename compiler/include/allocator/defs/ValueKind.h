#ifndef HAZE_VALUEKIND_H
#define HAZE_VALUEKIND_H

#include <utility/Sum.h>

// Haze ValueKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#include <allocator/defs/ValueKind.x>

	enum class ValueTag : TagType
	{
#define X(enumerator, name) enumerator,
		VALUE_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(ValueTag tag)
	{
		switch (tag)
		{
#define X(enumerator, name) case ValueTag::enumerator: return #name;
			VALUE_KINDS(X)
#undef X
		}

		return "<unknown value tag>";
	}
}

#endif
