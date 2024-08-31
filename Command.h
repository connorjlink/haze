#ifndef HAZE_COMMAND_H
#define HAZE_COMMAND_H

#include "Node.h"
#include "CommandType.h"

// Haze Command.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Command : public Node
	{
	public:
		std::uint32_t offset;

	public:
		Command(Token token)
			: Node{ token }
		{
		}

	public:
		virtual ~Command() = default;

	public:
		virtual NodeType ntype() const final override;
		virtual CommandType ctype() const = 0;
	};
}

#endif
