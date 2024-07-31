#ifndef HAZE_COMMANDTYPE_H
#define HAZE_COMMANDTYPE_H

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
}

#endif
