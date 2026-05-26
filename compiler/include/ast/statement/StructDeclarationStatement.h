#ifndef HAZE_STRUCTDECLARATIONSTATEMENT_H
#define HAZE_STRUCTDECLARATIONSTATEMENT_H

#include <ast/expression/Expression.h>

// Haze StructDeclarationStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class StructDeclarationStatement : public StatementBase
	{
	public:
		ExpressionReference<IdentifierExpression> identifier;
		std::vector<ExpressionReference<MemberDeclarationExpression>> members;

	public:
		std::uint16_t members_size() const;
		std::uint16_t struct_size() const;

	public:
		StructDeclarationStatement(IdentifierExpression* identifier, const std::vector<MemberDeclarationExpression*>& members, const Token& token)
			: StatementBase{ token }, identifier{ identifier }, members{ members }
		{
		}

	public:
		virtual StatementKind stype() const final override;
		virtual void generate(ValueHandle) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
