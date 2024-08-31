#ifndef HAZE_STRINGEXPRESSION_H
#define HAZE_STRINGEXPRESSION_H

#include "Expression.h"

// Haze StringExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class StringExpression : public Expression
	{
	public:
		std::string message;

	public:
		StringExpression(const std::string& message, Token token)
			: Expression{ token }, message{ message }
		{
		}
		
	public:
		virtual ExpressionType etype() const final override;
		virtual StringExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
