#ifndef HAZE_VARIABLESTATEMENT_H
#define HAZE_VARIABLESTATEMENT_H

#include "Statement.h"

// Haze VariableStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Type;
	class Allocation;
	class Expression;

	class VariableStatement : public Statement
	{
	private:
		Type* type;
		std::string name;
		Expression* value;
		Allocation* allocation;

	public:
		VariableStatement(Type* type, const std::string& name, Expression* value, Allocation* allocation, const Token& token)
			: Statement{ token }, type{ type }, name { name }, value{ value }, allocation{ allocation }
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
