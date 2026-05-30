#ifndef HAZE_FUNCTIONKIND_H
#define HAZE_FUNCTIONKIND_H

// Haze FunctionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#include <ast/function/defs/FunctionKind.x>

	enum class FunctionKind
	{
#define X(enumerator, type, name) enumerator,
		FUNCTION_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(FunctionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, type, name) case FunctionKind::type: return #name;
			FUNCTION_KINDS(X)
#undef X
		}

		return "<unknown function kind>";
	}
}

#endif
