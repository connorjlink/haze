#ifndef HAZE_FUNCTIONARGUMENTEXPRESSION_H
#define HAZE_FUNCTIONARGUMENTEXPRESSION_H

#include "ArgumentExpression.h"
#include "Type.h"

// Haze FunctionArgumentExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class FunctionArgumentExpression : public ArgumentExpression
	{
	public:
		Expression* value;

	public:
		FunctionArgumentExpression(Type* type, IdentifierExpression* identifier, Expression* value, Token token)
			: ArgumentExpression{ type, identifier, token }, value{ value }
		{
		}

	public:
		virtual ExpressionType etype() const final override;
		virtual FunctionArgumentExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
