#include "Function.h"
#include "Generator.h"
#include "Allocator.h"
#include "IdentifierExpression.h"
#include "ArgumentExpression.h"
#include "Utility.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "Parser.h"
#include "Log.h"

#include <format>
#include <cassert>

namespace hz
{
	NodeType Function::ntype() const
	{
		return NodeType::FUNCTION;
	}

	std::string Function::string() const
	{
		return std::format("function ({}())", name);
	}

	Function* Function::copy() const
	{
		return new Function{ *this };
	}

	void Function::generate(Allocation*)
	{
		_generator->begin_function(name);


		// TODO: support argument inputs here!
		const auto arity = arguments.size();

		if (arity > 0)
		{
			_generator->make_push(R0);
			_generator->make_push(R1);
			_generator->make_push(R2);
			_generator->make_push(R3);
		}
		

		if (arity > 4)
		{
			_error_reporter->post_uncorrectable(std::format("invalid argument count for function {}", name), NULL_TOKEN);
		}


		for (auto i = 0; i < arity; i++)
		{
			const auto address = _generator->fake_pull(static_cast<Register>(i), ARGUMENTS);
			// NOTE: this will not support recursion for now ;(
			// all arguments of of ->etype() == ExpressionType::ARGUMENT
			AS_ARGUMENT_SYMBOL(_parser->reference_symbol(SymbolType::ARGUMENT,
				AS_ARGUMENT_EXPRESSION(arguments[i])->identifier->name, NULL_TOKEN))->allocation = new DynamicAllocation{ address };
		}


		body->generate();

		// in case there was no return statement, a default return value of 0 is provided
		ManagedStaticAllocation temp{};
		_generator->make_copy(temp.allocation->read(), 0);
		_generator->fake_push(RETURNS, temp.allocation->read());

		if (arity > 0)
		{
			_generator->make_pull(R3);
			_generator->make_pull(R2);
			_generator->make_pull(R1);
			_generator->make_pull(R0);
		}

		_generator->make_exit();
	}

	Function* Function::optimize()
	{
		if (auto body_optimized = body->optimize())
		{
			return new Function{ name, return_type, std::move(arguments), AS_STATEMENT(body_optimized) };
		}

		return nullptr;
	}

	Node* Function::evaluate(Context* context) const
	{
		if (name == "main")
		{
			DISCARD body->evaluate(context);
			return nullptr;
		}

		else
		{
			const auto arguments_evaluated = context->pop_arguments();

			if (arguments.size() != arguments_evaluated.size())
			{
				// NOTE: this theoretically should never fire since we actually check 
				// the argument count during function call expression parsing
				Log::error(std::format("Function {} expected {} but got {} during evaluation", name, arguments.size(), arguments_evaluated.size()));
			}

			for (auto i = 0; i < arguments.size(); i++)
			{
				const auto argument_name = AS_IDENTIFIER_EXPRESSION(arguments[i])->name;
				context->define_variable(argument_name, harvest(arguments_evaluated[i]));
			}

			DISCARD body->evaluate(context);
			return nullptr;
		}
	}
}
