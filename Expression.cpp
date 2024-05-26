#include "Expression.h"

namespace hz
{
	Node::Type Expression::ntype() const
	{
		return Node::Type::EXPRESSION;
	}
}