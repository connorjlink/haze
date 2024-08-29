#ifndef HAZE_EXPRESSIONSTATEMENT_H
#define HAZE_EXPRESSIONSTATEMENT_H

#include "Statement.h"
#include "Expression.h"

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
		virtual std::string string() const final override;
		virtual ExpressionStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
