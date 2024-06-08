#ifndef HAZE_COMMAND_H
#define HAZE_COMMAND_H

#include "Node.h"

#define AS_COMMAND(x) static_cast<Command*>(x)

#define AS_DOTORG_COMMAND(x) static_cast<DotOrgCommand*>(x)
#define AS_DOTDEFINE_COMMAND(x) static_cast<DotDefineCommand*>(x)
#define AS_LABEL_COMMAND(x) static_cast<Label*>(x)
#define AS_INSTRUCTION_COMMAND(x) static_cast<Instruction*>(x)

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
		virtual ~Command() = default;

	public:
		virtual Node::Type ntype() const final override;
		virtual Command::Type ctype() const = 0;
	};
}

#endif
