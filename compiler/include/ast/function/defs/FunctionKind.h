#ifndef HAZE_FUNCTIONKIND_H
#define HAZE_FUNCTIONKIND_H

// Haze FunctionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class FunctionKind
	{
#define X(enumerator, type, name) enumerator,
#include <ast/statement/FunctionKind.x>
#undef X
	};

	constexpr std::string_view to_string(FunctionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, type, name) case FunctionKind::type: return #name;
#include <ast/statement/FunctionKind.x>
#undef X
		}

		return "<unknown function kind>";
	}
}

#endif
