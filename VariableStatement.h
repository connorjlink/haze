#ifndef HAZE_VARIABLESTATEMENT_H
#define HAZE_VARIABLESTATEMENT_H

#include "Statement.h"
#include "Expression.h"

namespace hz
{
	class VariableStatement : public Statement
	{
	private:
		std::string_view name;
		Expression* value;
		Allocation* allocation;

	public:
		VariableStatement(std::string_view name, Expression* value, Allocation* allocation)
			: name(name), value(value), allocation(allocation)
		{
		}

	public:
		virtual Statement::Type stype() const final override;
		virtual std::string string() const final override;
		virtual VariableStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
	};
}

#endif
