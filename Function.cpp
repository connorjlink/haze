#include "Function.h"
#include "Generator.h"
#include "Allocator.h"
#include "IdentifierExpression.h"
#include "ArgumentExpression.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "Parser.h"
#include "Statement.h"
#include "Symbol.h"
#include "ErrorReporter.h"

import std;

// Haze Function.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Function::Function(std::string name, TypeSpecifier return_type, std::vector<Expression*>&& arguments, Statement* body, Token token)
		: Node{ token }, name{ std::move(name) }, return_type{ return_type }, arguments{ std::move(arguments) }, body{ body }
	{
	}

	NodeType Function::ntype() const
	{
		return NodeType::FUNCTION;
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

		//if (arity > 0)
		//{
		//	for (auto i = 0; i < arity; i++)
		//	{
		//		_generator->make_push(static_cast<Register>(i));
		//	}
		//}
		

		if (arity > 4)
		{
			_error_reporter->post_uncorrectable(std::format("invalid argument count for function {}", name), NULL_TOKEN);
		}


		for (int i = 0; i < arity; i++)
		{
			auto reg = static_cast<Register>(i);

			_generator->make_pull(static_cast<Register>(arity - i - 1));
			// NOTE: this will not support recursion for now ;(
			// all arguments are of ->etype() == ExpressionType::ARGUMENT
			AS_ARGUMENT_SYMBOL(_parser->reference_symbol(SymbolType::ARGUMENT,
				AS_ARGUMENT_EXPRESSION(arguments[i])->identifier->name, NULL_TOKEN))->allocation = new StaticAllocation{ reg, false };
		}


		body->generate();

		// in case there was no return statement, a default return value of 0 is provided
		if constexpr (0)
		{
			ManagedStaticAllocation temp{};
			_generator->make_copy(temp.allocation->read(), 0);
			_generator->make_push(temp.allocation->read());
		}
		

		//if (arity > 0)
		//{
		//	for (auto i = arity - 1; i >= 0; i--)
		//	{
		//		_generator->make_pull(static_cast<Register>(i));
		//	}
		//}

		_generator->make_exit(0);
	}

	Function* Function::optimize()
	{
		if (auto body_optimized = body->optimize())
		{
			return new Function{ name, return_type, std::move(arguments), AS_STATEMENT(body_optimized), _token };
		}

		return nullptr;
	}

	Node* Function::evaluate(Context* context) const
	{
		if (name == "main")
		{
			body->evaluate(context);
			return nullptr;
		}

		else
		{
			const auto arguments_evaluated = context->pop_arguments();

			for (auto i = 0; i < arguments.size(); i++)
			{
				const auto argument_name = AS_IDENTIFIER_EXPRESSION(arguments[i])->name;
				context->define_variable(argument_name, harvest(arguments_evaluated[i]));
			}

			body->evaluate(context);
			return nullptr;
		}
	}
}
