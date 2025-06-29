#ifndef HAZE_EXPRESSION_H
#define HAZE_EXPRESSION_H

#include "Node.h"
#include "ExpressionType.h"

// Haze ExpressionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Expression : public Node
	{
	public:
		Expression(const Token& token)
			: Node{ token }
		{
		}

	public:
		virtual NodeType ntype() const final override;
		virtual ExpressionType etype() const = 0;
	};
}

#endif
