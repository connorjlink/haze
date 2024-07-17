#ifndef HAZE_EXPRESSION_H
#define HAZE_EXPRESSION_H

#include "Node.h"

#include "ExpressionType.h"

namespace hz
{
	class Expression : public Node
	{
	public:
		virtual ~Expression() = default;

	public:
		virtual Node::Type ntype() const final override;
		virtual ExpressionType etype() const = 0;
	};
}

#endif //HAZE_EXPRESSION_H
