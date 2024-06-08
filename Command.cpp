#include "Command.h"

namespace hz
{
	Node::Type Command::ntype() const
	{
		return Node::Type::COMMAND;
	}
}