#ifndef HAZE_STRUCTDECLARATIONSTATEMENT_H
#define HAZE_STRUCTDECLARATIONSTATEMENT_H

#include "Statement.h"

// Haze StructDeclarationStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IdentifierExpression;
	class MemberDeclarationExpression;

	class StructDeclarationStatement : public Statement
	{
	public:
		IdentifierExpression* identifier;
		std::vector<MemberDeclarationExpression*> members;

	public:
		std::uint16_t members_size() const;
		std::uint16_t struct_size() const;

	public:
		StructDeclarationStatement(IdentifierExpression* identifier, const std::vector<MemberDeclarationExpression*>& members, const Token& token)
			: Statement{ token }, identifier{ identifier }, members{ members }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual StructDeclarationStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
