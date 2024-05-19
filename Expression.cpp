#include "Expression.h"

namespace hz
{
	Node::Type Expression::ntype() const
	{
		return Node::Type::EXPRESSION;
	}

	Expression* Expression::optimize()
	{
		return nullptr;
	}
}