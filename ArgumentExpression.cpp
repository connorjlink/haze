#include "ArgumentExpression.h"

namespace hz
{
	ExpressionType ArgumentExpression::etype() const
	{
		return ExpressionType::ARGUMENT;
	}

	std::string ArgumentExpression::string() const
	{
		return "argument expression";
	}

	ArgumentExpression* ArgumentExpression::copy() const
	{
		return new ArgumentExpression{ *this };
	}

	void ArgumentExpression::generate(Allocation*)
	{
#pragma message("TODO: compiler code generation for argument expression")
	}

	Expression* ArgumentExpression::optimize()
	{
		return nullptr;
	}

	Node* ArgumentExpression::evaluate(Context* context) const
	{
		return identifier->copy();
	}
}