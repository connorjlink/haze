import std;

#include <allocator/Allocation.h>
#include <ast/FunctionCallExpression.h>
#include <ast/FunctionArgumentExpression.h>
#include <ast/ArgumentExpression.h>
#include <ast/Function.h>
#include <runtime/Context.h>
#include <runtime/Evaluator.h>
#include <symbol/Symbol.h>
#include <toolchain/Generator.h>
#include <type/TypeCheck.h>

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
		auto symbol = USE_SAFE(SymbolDatabase)->reference_symbol(SymbolType::FUNCTION, name, _token, true);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		const auto defined_arity = function_symbol->arity();
		const auto called_arity = arguments.size();

		if (defined_arity != called_arity)
		{
			USE_UNSAFE(ErrorReporter)->post_error(std::format(
				"function `{}` expects {} arguments but got {}", name, defined_arity, called_arity), _token);
			return;
		}

		std::vector<Expression*> argument_expressions{};

		for (auto i = 0; i < arguments.size(); i++)
		{
			auto corresponding_argument = function_symbol->arguments[i];
			auto argument_expression = AS_ARGUMENT_EXPRESSION(corresponding_argument);

			Type* type = nullptr;

			if (check_type(arguments[i], argument_expression->type) == true)
			{
				type = argument_expression->type;
			}

			argument_expressions.emplace_back(new FunctionArgumentExpression
				{ type, argument_expression->identifier, arguments[i], argument_expression->_token });
		}

		// will emit a placeholder call address before hot-patching in the correct target during linking
		REQUIRE_SAFE(Generator)->call_function(name, argument_expressions, allocation);
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
				USE_UNSAFE(ErrorReporter)->post_error(
					"function call arguments must evaluate to an r-value", argument->_token);
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
				return variable_to_node(return_value);
			}
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"function `{}` is undefined", name), _token);
		return nullptr;
	}
}