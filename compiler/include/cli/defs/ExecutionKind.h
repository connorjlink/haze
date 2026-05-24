#ifndef HAZE_EXECUTIONKIND_H
#define HAZE_EXECUTIONKIND_H

// Haze ExecutionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ExecutionKind
	{
#define X(enumerator, name) enumerator,
#include <cli/defs/ExecutionKind.x>
#undef X
	};

	constexpr std::string_view to_string(ExecutionKind execution)
	{
		switch (execution)
		{
#define X(enumerator, name) case ExecutionKind::enumerator: return #name;
#include <cli/defs/ExecutionKind.x>
#undef X
		}

		return "<unknown execution kind>";
	}
}

#endif
