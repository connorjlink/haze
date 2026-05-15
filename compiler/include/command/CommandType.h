#ifndef HAZE_COMMANDTYPE_H
#define HAZE_COMMANDTYPE_H

// Haze CommandType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_COMMAND(x) static_cast<Command*>(x)

#define AS_LABEL_COMMAND(x) static_cast<LabelCommand*>(x)
#define AS_INSTRUCTION_COMMAND(x) static_cast<InstructionCommand*>(x)

namespace hz
{
	enum class CommandType
	{
		LABEL,
		INSTRUCTION,
	};

	static const std::unordered_map<CommandType, std::string_view> _command_type_map
	{
		{ CommandType::LABEL, "label" },
		{ CommandType::INSTRUCTION, "instruction" },
	};
}

#endif
