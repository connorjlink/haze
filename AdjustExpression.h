#ifndef HAZE_ADJUSTEXPRESSION_H
#define HAZE_ADJUSTEXPRESSION_H

#include "Expression.h"

// Haze AdjustExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AdjustExpression : public Expression
	{
	private:
		// increment if true; decrement if false
		bool increment;
		Expression* target;

	public:
		AdjustExpression(bool increment, Expression* target, Token token)
			: Expression{ token }, increment{ increment }, target{ target }
		{
		}

	public:
		virtual ExpressionType etype() const final override;
		virtual AdjustExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
