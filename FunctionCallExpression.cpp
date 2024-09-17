#include "FunctionCallExpression.h"
#include "Parser.h"
#include "Allocation.h"
#include "Generator.h"
#include "Evaluator.h"
#include "Symbol.h"
#include "ErrorReporter.h"
#include "X86Builder.h"

import std;

// Haze FunctionCallExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ExpressionType FunctionCallExpression::etype() const
	{
		return ExpressionType::FUNCTION_CALL;
	}

	FunctionCallExpression* FunctionCallExpression::copy() const
	{
		return new FunctionCallExpression{ *this };
	}

	void FunctionCallExpression::generate(Allocation* allocation)
	{
		auto symbol = AS_FUNCTION_SYMBOL(_parser->reference_symbol(SymbolType::FUNCTION, name, _token, true));

		const auto defined_arity = symbol->arity;
		const auto called_arity = arguments.size();

		if (defined_arity != called_arity)
		{
			_error_reporter->post_error(std::format("function `{}` expects {} arguments but got {}", name, defined_arity, called_arity), _token);
			return;
		}

		for (auto argument : arguments)
		{
			argument->generate(allocation);
			_generator->make_argument(allocation->read());
		}

		// Placeholder call address before we hot-patch in the correct target during linking
		_generator->call_function(name);

		// Return values always come in the EAX register
		_generator->make_copy(allocation->read(), EAX);
	}

	Expression* FunctionCallExpression::optimize()
	{
#pragma message("TODO: implement determination if a function is constexpr")
		//in that case, we can fold it down to a single integer literal node
		//for now, we don't have that functionality so just terminate further optimization of this node
		return nullptr;
	}

	Node* FunctionCallExpression::evaluate(Context* context) const
	{
		std::vector<Expression*> arguments_evaluated{};

		for (auto& argument : arguments)
		{
			const auto argument_evaluated = argument->evaluate(context);

			if (argument_evaluated->ntype() != NodeType::EXPRESSION)
			{
				_error_reporter->post_error("function call arguments must evaluate to an r-value", argument->_token);
			}

			arguments_evaluated.emplace_back(AS_EXPRESSION(argument_evaluated));
		}

		for (auto& function : context->functions())
		{
			if (function->name == name)
			{
				context->push_arguments(arguments_evaluated);
				function->evaluate(context);

				const auto return_value = context->pop_return();
				return unharvest(return_value);
			}
		}

		_error_reporter->post_error(std::format("function `{}` is undefined", name), _token);
		return nullptr;
	}
}