#ifndef HAZE_RETURNSTATEMENT_H
#define HAZE_RETURNSTATEMENT_H

#include <ast/expression/Expression.h>
#include <ast/statement/Statement.h>
#include <allocator/Value.h>

// Haze ReturnStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ReturnStatement : public StatementBase
	{
	private:
		std::string enclosing_function;
		ExpressionHandle expression;
		ValueHandle value;

	public:
		ReturnStatement(const std::string& enclosing_function, ExpressionHandle expression, ValueHandle value, const Token& token)
			: StatementBase{ token }, enclosing_function{ enclosing_function }, expression{ expression }, value{ value }
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
