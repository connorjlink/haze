#ifndef HAZE_COMMAND_H
#define HAZE_COMMAND_H

#include "Node.h"

#include "CommandType.h"

namespace hz
{
	class Command : public Node
	{
	public:
		std::uint16_t offset;

	public:
		virtual ~Command() = default;

	public:
		virtual NodeType ntype() const final override;
		virtual CommandType ctype() const = 0;
	};
}

#endif
