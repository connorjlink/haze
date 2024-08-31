#ifndef HAZE_IDENTIFIEREXPRESSION_H
#define HAZE_IDENTIFIEREXPRESSION_H

#include "Expression.h"

// Haze IdentifierExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IdentifierExpression : public Expression
	{
	public:
		std::string name;

	public:
		IdentifierExpression(const std::string& name, Token token)
			: Expression{ token }, name { name }
		{
		}

	public:
		virtual ExpressionType etype() const final override;
		virtual IdentifierExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
