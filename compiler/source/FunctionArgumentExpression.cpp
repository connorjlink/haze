import std;

#include <ast/expression/Expression.h>

// Haze FunctionArgumentExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType FunctionArgumentExpression::etype() const
	{
		return ExpressionType::FUNCTION_ARGUMENT;
	}

	ExpressionHandle FunctionArgumentExpression::optimize()
	{
		if (auto value_optimized = value->optimize())
		{
			return new FunctionArgumentExpression{ type, identifier, AS_EXPRESSION_NODE(value_optimized), _token };
		}

		return nullptr;
	}

	void FunctionArgumentExpression::generate(ValueHandle allocation)
	{
		value->generate(allocation);
	}

	Node* FunctionArgumentExpression::evaluate(Context*) const
	{
		USE_SAFE(ErrorReporter)->post_error("unsupported interpreter expression type `function argument`", _token);
		return nullptr;
	}
}
