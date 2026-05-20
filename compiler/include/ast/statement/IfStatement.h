#ifndef HAZE_IFSTATEMENT_H
#define HAZE_IFSTATEMENT_H

#include <ast/expression/Expression.h>
#include <ast/statement/Statement.h>

// Haze IfStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IfStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle if_body;
		StatementHandle else_body;

	public:
		IfStatement(Expression* condition, StatementHandle if_body, StatementHandle else_body, const Token& token)
			: StatementBase{ token }, condition{ condition }, if_body{ if_body }, else_body{ else_body }
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
