#ifndef HAZE_COMMANDTYPE_H
#define HAZE_COMMANDTYPE_H

// Haze CommandType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_COMMAND(x) static_cast<Command*>(x)

#define AS_DOTORG_COMMAND(x) static_cast<DotOrgCommand*>(x)
#define AS_DOTDEFINE_COMMAND(x) static_cast<DotDefineCommand*>(x)
#define AS_LABEL_COMMAND(x) static_cast<LabelCommand*>(x)
#define AS_INSTRUCTION_COMMAND(x) static_cast<InstructionCommand*>(x)

namespace hz
{
	enum class CommandType
	{
		DOTORG,
		DOTDEFINE,
		LABEL,
		INSTRUCTION,
	};

	static const std::unordered_map<CommandType, std::string> _command_type_map
	{
		{ CommandType::DOTORG, ".org" },
		{ CommandType::DOTDEFINE, ".define" },
		{ CommandType::LABEL, "label" },
		{ CommandType::INSTRUCTION, "instruction" },
	};
}

#endif
