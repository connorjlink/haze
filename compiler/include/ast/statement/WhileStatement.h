#ifndef HAZE_WHILESTATEMENT_H
#define HAZE_WHILESTATEMENT_H

#include <ast/expression/Expression.h>
#include <ast/statement/Statement.h>

// Haze WhileStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class WhileStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle body;

	public:
		WhileStatement(ExpressionHandle condition, StatementHandle body, const Token& token)
			: StatementBase{ token }, condition{ condition }, body{ body }
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
