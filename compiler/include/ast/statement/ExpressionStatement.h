#ifndef HAZE_EXPRESSIONSTATEMENT_H
#define HAZE_EXPRESSIONSTATEMENT_H

#include <ast/expression/Expression.h>
#include <ast/statement/Statement.h>

// Haze ExpressionStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ExpressionStatement : public StatementBase
	{
	private:
		ExpressionHandle expression;

	public:
		ExpressionStatement(ExpressionHandle expression, const Token& token)
			: StatementBase{ token }, expression{ expression }
		{
		}

	public:
		StatementType tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		StatementHandle get_type(const Storage&) const;
	};
}

#endif 
