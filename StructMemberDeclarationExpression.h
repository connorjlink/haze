#ifndef HAZE_STRUCTMEMBERDECLARATIONEXPRESSION_H
#define HAZE_STRUCTMEMBERDECLARATIONEXPRESSION_H

#include "Expression.h"
#include "Type.h"

// Haze StructMemberDeclarationExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IdentifierExpression;

	class StructMemberDeclarationExpression : public Expression
	{
	public:
		Type* type;
		IdentifierExpression* identifier;

	public:
		StructMemberDeclarationExpression(Type* type, IdentifierExpression* identifier, Token token)
			: Expression{ token }, type{ type }, identifier{ identifier }
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
