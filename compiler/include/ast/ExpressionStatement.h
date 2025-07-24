#ifndef HAZE_EXPRESSIONSTATEMENT_H
#define HAZE_EXPRESSIONSTATEMENT_H

#include "Statement.h"
#include "Expression.h"

// Haze ExpressionStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ExpressionStatement : public Statement
	{
	private:
		Expression* expression;

	public:
		ExpressionStatement(Expression* expression, Token token)
			: Statement{ token }, expression{ expression }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual ExpressionStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
