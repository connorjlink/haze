#ifndef HAZE_ARGUMENTEXPRESSION_H
#define HAZE_ARGUMENTEXPRESSION_H

#include "Expression.h"
#include "IdentifierExpression.h"

// Haze ArgumentExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeSpecifier;

	class ArgumentExpression : public Expression
	{
	public:
		TypeSpecifier type_specifier;
		IdentifierExpression* identifier;

	public:
		ArgumentExpression(TypeSpecifier type_specifier, IdentifierExpression* identifier, Token token)
			: Expression{ token }, type_specifier { type_specifier }, identifier{ identifier }
		{
		}

	public:
		virtual ExpressionType etype() const final override;
		virtual ArgumentExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context* context) const final override;
	}; 
}

#endif 
