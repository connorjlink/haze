#ifndef HAZE_FORSTATEMENT_H
#define HAZE_FORSTATEMENT_H

#include "Statement.h"
#include "Expression.h"

// Haze ForStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ForStatement : public Statement
	{
	private:
		Statement* initialization;
		Expression* condition;
		Expression* expression;
		Statement* body;

	public:
		ForStatement(Statement* initialization, Expression* condition, Expression* expression, Statement* body, Token token)
			: Statement{ token }, initialization{ initialization }, condition{condition}, expression{expression}, body{body}
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual ForStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
