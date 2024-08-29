#ifndef HAZE_VARIABLESTATEMENT_H
#define HAZE_VARIABLESTATEMENT_H

#include "Statement.h"
#include "Expression.h"
#include "Allocation.h"

namespace hz
{
	enum class ReturnType;

	class VariableStatement : public Statement
	{
	private:
		ReturnType type;
		std::string name;
		Expression* value;
		Allocation* allocation;

	public:
		VariableStatement(ReturnType type, std::string name, Expression* value, Allocation* allocation, Token token)
			: Statement{ token }, type{ type }, name { std::move(name) }, value{ value }, allocation{ allocation }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual std::string string() const final override;
		virtual VariableStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
