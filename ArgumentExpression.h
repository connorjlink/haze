#ifndef HAZE_ARGUMENTEXPRESSION_H
#define HAZE_ARGUMENTEXPRESSION_H

#include "Expression.h"
#include "IdentifierExpression.h"

// Haze ArgumentExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Type;

	class ArgumentExpression : public Expression
	{
	public:
		Type* type;
		IdentifierExpression* identifier;

	public:
		ArgumentExpression(Type* type, IdentifierExpression* identifier, Token token)
			: Expression{ token }, type{ type }, identifier{ identifier }
		{
		}

	public:
		virtual ExpressionType etype() const override;
		virtual ArgumentExpression* copy() const override;
		virtual void generate(Allocation*) override;
		virtual Expression* optimize() override;
		virtual Node* evaluate(Context* context) const override;
	}; 
}

#endif 
