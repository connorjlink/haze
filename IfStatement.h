#ifndef HAZE_IFSTATEMENT_H
#define HAZE_IFSTATEMENT_H

#include "Expression.h"
#include "Statement.h"

namespace hz
{
	class IfStatement : public Statement
	{
	private:
		Expression* condition;
		Statement* if_body;
		Statement* else_body;

	public:
		IfStatement(Expression* condition, Statement* if_body, Statement* else_body, Token token)
			: Statement{ token }, condition{ condition }, if_body{ if_body }, else_body{ else_body }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual std::string string() const final override;
		virtual IfStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
