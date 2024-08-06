#ifndef HAZE_WHILESTATEMENT_H
#define HAZE_WHILESTATEMENT_H

#include "Expression.h"
#include "Statement.h"

namespace hz
{
	class WhileStatement : public Statement
	{
	private:
		Expression* condition;
		Statement* body;

	public:
		WhileStatement(Expression* condition, Statement* body)
			: condition{ condition }, body{ body }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual std::string string() const final override;
		virtual WhileStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
