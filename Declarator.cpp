#include "Declarator.h"

namespace hz
{
	Node::Type Declarator::ntype() const
	{
		return Node::Type::DECLARATOR;
	}
}