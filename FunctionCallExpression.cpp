#include "FunctionCallExpression.h"
#include "Parser.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Utility.h"
#include "Evaluator.h"
#include "ErrorReporter.h"
#include "Log.h"

#include <format>

namespace hz
{
	ExpressionType FunctionCallExpression::etype() const
	{
		return ExpressionType::FUNCTION_CALL;
	}

	std::string FunctionCallExpression::string() const
	{
		return std::format("function call ({})", name);
	}

	FunctionCallExpression* FunctionCallExpression::copy() const
	{
		return new FunctionCallExpression{ *this };
	}

	void FunctionCallExpression::generate(Allocation* allocation)
	{
		auto symbol = AS_FUNCTION_SYMBOL(_parser->reference_symbol(SymbolType::FUNCTION, name, NULL_TOKEN, true));

		const auto defined_arity = symbol->arity;
		const auto called_arity = arguments.size();

		if (defined_arity != called_arity)
		{
			_error_reporter->post_error(std::format("function `{}` expects {} arguments but got {}", name, defined_arity, called_arity), NULL_TOKEN);
			return;
		}

		if (!arguments.empty())
		{
			ManagedStaticAllocation temp{ allocation->read() };

			for (auto argument : arguments)
			{
				argument->generate(temp.allocation);
				_generator->fake_push(ARGUMENTS, temp.allocation->read());
			}
		}

		//Placeholder call address before we hot-patch in the correct target after linking
		_generator->make_call(name);

		// return value comes off fake stack
		_generator->fake_pull(allocation->read(), RETURNS);
	}

	Expression* FunctionCallExpression::optimize()
	{
		//TODO: implement determination if a function is constexpr
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
				Log::error("Function call arguments must evaluate to a single expression");
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

		Log::error(std::format("function {} is undefined and cannot be called", name));
	}
}