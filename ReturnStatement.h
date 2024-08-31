#ifndef HAZE_RETURNSTATEMENT_H
#define HAZE_RETURNSTATEMENT_H

#include "Statement.h"
#include "Function.h"
#include "Expression.h"
#include "Context.h"

// Haze ReturnStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ReturnStatement : public Statement
	{
	private:
		std::string enclosing_function;
		Expression* value;
		Allocation* allocation;

	public:
		ReturnStatement(std::string enclosing_function, Expression* value, Allocation* allocation, Token token)
			: Statement{ token }, enclosing_function{ std::move(enclosing_function) }, value{ value }, allocation{ allocation }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual ReturnStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
