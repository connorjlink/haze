#ifndef HAZE_VARIABLESTATEMENT_H
#define HAZE_VARIABLESTATEMENT_H

#include "Statement.h"
#include "Expression.h"

// Haze VariableStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeSpecifier;
	class Allocation;

	class VariableStatement : public Statement
	{
	private:
		TypeSpecifier type;
		std::string name;
		Expression* value;
		Allocation* allocation;

	public:
		VariableStatement(TypeSpecifier type, std::string name, Expression* value, Allocation* allocation, Token token)
			: Statement{ token }, type{ type }, name { std::move(name) }, value{ value }, allocation{ allocation }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual VariableStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
