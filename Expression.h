#ifndef HAZE_EXPRESSION_H
#define HAZE_EXPRESSION_H

#include "Node.h"

#include "ExpressionType.h"

namespace hz
{
	class Expression : public Node
	{
	public:
		Expression(Token token)
			: Node{ token }
		{
		}

	public:
		virtual ~Expression() = default;

	public:
		virtual NodeType ntype() const final override;
		virtual ExpressionType etype() const = 0;
	};
}

#endif //HAZE_EXPRESSION_H
