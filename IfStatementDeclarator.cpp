#include "IfStatementDeclarator.h"

#include <format>

namespace hz
{
	StatementDeclarator::Type IfStatementDeclarator::stype() const
	{
		return StatementDeclarator::Type::IF;
	}

	std::string IfStatementDeclarator::string() const
	{
		return std::format("if statement declarator ({})", statement->string());
	}

	IfStatementDeclarator* IfStatementDeclarator::copy() const
	{
		return new IfStatementDeclarator{ *this };
	}

	void IfStatementDeclarator::generate(Allocation*)
	{
		// TODO: nothing to do here??

		// A possible option is to re-utilize generate() as 
		// an equivalent to evaluate() for the interpreter context!
	}

	StatementDeclarator* IfStatementDeclarator::optimize()
	{
		auto statement_optimized = AS_IF_STATEMENT(statement->optimize());

		// TODO: we should be able to optimize away if statements that have compile-time-known conditions!
		// but for now we will suffice by just optimizing the condition expression and body statement

		if (statement_optimized != nullptr)
		{
			return new IfStatementDeclarator{ statement_optimized };
		}

		return nullptr;
	}
}