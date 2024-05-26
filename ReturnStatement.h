#ifndef HAZE_RETURNSTATEMENT_H
#define HAZE_RETURNSTATEMENT_H

#include "Statement.h"
#include "Expression.h"

namespace hz
{
	class ReturnStatement : public Statement
	{
	private:
		Expression* value;
		Allocation* allocation;

	public:
		ReturnStatement(Expression* value, Allocation* allocation)
			: value(value), allocation(allocation)
		{
		}

	public:
		virtual Statement::Type stype() const final override;
		virtual std::string string() const final override;
		virtual ReturnStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
	};
}

#endif
