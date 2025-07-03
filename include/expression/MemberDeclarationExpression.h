#ifndef HAZE_MEMBERDECLARATIONEXPRESSION_H
#define HAZE_MEMBERDECLARATIONEXPRESSION_H

#include "Expression.h"
#include "Type.h"

// Haze MemberDeclarationExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IdentifierExpression;

	class MemberDeclarationExpression : public Expression
	{
	public:
		Type* type;
		IdentifierExpression* identifier;

	public:
		std::uint16_t member_size() const;

	public:
		MemberDeclarationExpression(Type* type, IdentifierExpression* identifier, const Token& token)
			: Expression{ token }, type{ type }, identifier{ identifier }
		{
		}

	public:
		virtual ExpressionType etype() const final override;
		virtual MemberDeclarationExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
