#include "Function.h"
#include "Generator.h"
#include "Allocator.h"
#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "ArgumentExpression.h"
#include "ReturnStatement.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "Parser.h"
#include "Statement.h"
#include "Symbol.h"
#include "X86Builder.h"
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
		_generator->begin_scope();

		const auto arity = arguments.size();

#pragma message("TODO: function argument pulling using take_argument()")

		// create a register allocation for every one of our arguments
		std::vector<AutoStackAllocation> argument_allocations{ arity };

		for (auto i = 0; i < arity; i++)
		{
			// NOTE: the 0-th local variable is stored at [ebp - 4]
#pragma message("TODO: use a different size other than 4 for non-32-bit values (custom types)")
			_generator->take_argument(argument_allocations[i].source()->read(), (i + 1) * 4);

			// TODO: does this support recursion properly
			// all arguments are of ->etype() == ExpressionType::ARGUMENT
			AS_ARGUMENT_SYMBOL(_parser->reference_symbol(SymbolType::ARGUMENT,
				AS_ARGUMENT_EXPRESSION(arguments[i])->identifier->name, NULL_TOKEN))->allocation 
					= argument_allocations[i].source();
		}

		body->generate();
		// NOTE: opting to generate the function epilogue for every single return statement

		// in case there was no return statement, a default return value of 0 is provided
#pragma message("TODO: figure out if there was not a return statement")
		if constexpr (false)
		{
			_error_reporter->post_warning("implicit function return generated", body->_token);
			
			Expression* return_value = nullptr;

			using enum TypeSpecifier;
			switch (return_type)
			{
				case BYTE: return_value = new IntegerLiteralExpression{ 0, NULL_TOKEN }; break;
				default: _error_reporter->post_error("invalid return type", _token); return;
			}

			auto implicit_return = new ReturnStatement{ name, return_value, new ObserverAllocation{ EAX }, NULL_TOKEN};
			// NOTE: generating a statement, so no need to pass an allocation
			implicit_return->generate(nullptr);
		}
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
