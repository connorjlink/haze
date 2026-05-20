#ifndef HAZE_FORSTATEMENT_H
#define HAZE_FORSTATEMENT_H

#include <ast/expression/Expression.h>
#include <ast/statement/Statement.h>

// Haze ForStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ForStatement : public StatementBase
	{
	private:
		StatementHandle initialization;
		ExpressionHandle condition;
		ExpressionHandle induction;
		StatementHandle body;

	public:
		ForStatement(StatementHandle initialization, ExpressionHandle condition, ExpressionHandle induction, StatementHandle body, const Token& token)
			: StatementBase{ token }, initialization{ initialization }, condition{ condition }, induction{ induction }, body{ body }
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
