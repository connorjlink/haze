#ifndef HAZE_COMMANDTYPE_H
#define HAZE_COMMANDTYPE_H

// Haze CommandType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class CommandKind
	{
		LABEL,
		INSTRUCTION,
		DOTORG,
		DOTDEFINE,
	};

	static const std::unordered_map<CommandKind, std::string_view> command_kind_map
	{
		{ CommandKind::LABEL, "label" },
		{ CommandKind::INSTRUCTION, "instruction" },
		{ CommandKind::DOTORG, ".org" },
		{ CommandKind::DOTDEFINE, ".define" },
	};
}

#endif
