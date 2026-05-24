#ifndef HAZE_COMMANDKIND_H
#define HAZE_COMMANDKIND_H

// Haze CommandKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class CommandKind
	{
#define X(enumerator, name) enumerator,
#include <command/defs/CommandKind.x>
#undef X
	};

	constexpr std::string_view to_string(CommandKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case CommandKind::enumerator: return #name;
#include <command/defs/CommandKind.x>
#undef X
		}

		return "<unknown command kind>";
	}
}

#endif
