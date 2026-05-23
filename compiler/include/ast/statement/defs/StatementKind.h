#ifndef HAZE_STATEMENTKIND_H
#define HAZE_STATEMENTKIND_H

// Haze StatementKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class StatementKind
	{
#define X(enumerator, type, name) enumerator,
#include <ast/statement/StatementKind.def>
#undef X
	};

	constexpr std::string_view to_string(StatementKind kind)
	{
		switch (kind)
		{
#define X(enumerator, type, name) case StatementKind::type: return #name;
#include <ast/statement/StatementKind.def>
#undef X
		}

		return "<unknown statement kind>";
	}
}

#endif