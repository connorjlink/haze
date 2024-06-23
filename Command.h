#ifndef HAZE_COMMAND_H
#define HAZE_COMMAND_H

#include "Node.h"

#define AS_COMMAND(x) static_cast<Command*>(x)

#define AS_DOTORG_COMMAND(x) static_cast<DotOrgCommand*>(x)
#define AS_DOTDEFINE_COMMAND(x) static_cast<DotDefineCommand*>(x)
#define AS_LABEL_COMMAND(x) static_cast<LabelCommand*>(x)
#define AS_INSTRUCTION_COMMAND(x) static_cast<InstructionCommand*>(x)

namespace hz
{
	class Command : public Node
	{
	public:
		enum class Type
		{
			DOTORG,
			DOTDEFINE,
			LABEL,
			INSTRUCTION,
		};

	public:
		std::uint16_t offset;

	public:
		virtual ~Command() = default;

	public:
		virtual Node::Type ntype() const final override;
		virtual Command::Type ctype() const = 0;
	};
}

#endif
