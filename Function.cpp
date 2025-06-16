import std;

#include "Function.h"
#include "Generator.h"
#include "Allocator.h"
#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "ArgumentExpression.h"
#include "CompoundStatement.h"
#include "ReturnStatement.h"
#include "Allocation.h"
#include "Evaluator.h"
#include "Parser.h"
#include "CompilerParser.h"
#include "Statement.h"
#include "Symbol.h"
#include "X86Builder.h"
#include "CommonErrors.h"
#include "ErrorReporter.h"

// Haze Function.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Function::Function(const std::string& name, Type* return_type, std::vector<Expression*>&& arguments, Statement* body, const Token& token)
		: Node{ token }, name{ name }, return_type{ return_type }, arguments{ std::move(arguments) }, body{ body }
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

		// ensure each function implicitly has its own scope block
		if (body->stype() != StatementType::COMPOUND)
		{
			body = new CompoundStatement{ { body }, body->_token };
		}

		const auto arity = arguments.size();

		auto symbol = _parser->reference_symbol(SymbolType::FUNCTION, name, _token);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		_generator->begin_scope();

		auto current_argument = 0;
		for (auto argument : arguments)
		{
			auto argument_expression = AS_ARGUMENT_EXPRESSION(argument);
			_generator->attach_local(argument_expression->identifier->name, ((current_argument + 2) * 4));

			current_argument++;
		}


		// NOTE: old method
		// create a register allocation for every one of our arguments
		/*std::vector<AutoStackAllocation> argument_allocations{arity};

		for (auto i = 0; i < arity; i++)
		{
			auto argument_expression = AS_ARGUMENT_EXPRESSION(arguments[i]);

			// NOTE: the 0-th local variable is stored at [ebp - 4]
#pragma message("TODO: use a different size other than 4 for non-32-bit values (custom types)")

			_generator->take_argument(argument_expression->identifier->name, 
				argument_allocations[i].source()->read(), (i + 2) * 4);

			// TODO: does this support recursion properly
			// all arguments are of ->etype() == ExpressionType::ARGUMENT
			AS_ARGUMENT_SYMBOL(_parser->reference_symbol(SymbolType::ARGUMENT,
				argument_expression->identifier->name, NULL_TOKEN))->allocation 
					= argument_allocations[i].source();
		}*/

		body->generate();

		//_generator->branch_label();

		// NOTE: old method
		//_generator->end_scope();

		if (_parser->ptype() == ParserType::COMPILER)
		{
			auto compiler_parser = AS_COMPILER_PARSER(_parser);

			const auto end_function_label = compiler_parser->_function_label_map.at(name);
			_generator->branch_label(end_function_label);

			_generator->end_scope();
		}

		else
		{
			CommonErrors::invalid_parser_type(_parser->ptype(), _token);
			return;
		}
		

		// NOTE: opting to generate the function epilogue for every single return statement
		// in case there was no return statement, a default return value of 0 is provided
#pragma message("TODO: figure out if there was not a return statement")
		if constexpr (false)
		{
			_error_reporter->post_warning(std::format("implicit return generated for function `{}`", name), body->_token);
			
			Expression* return_value = nullptr;

#pragma message("TODO: determin fi the return type is valid or not")
			/*using enum TypeSpecifier;
			switch (return_type)
			{
				case BYTE: return_value = new IntegerLiteralExpression{ 0, NULL_TOKEN }; break;
				default: _error_reporter->post_error("invalid return type", _token); return;
			}*/

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
				context->define_variable(argument_name, node_to_variable(arguments_evaluated[i]));
			}

			body->evaluate(context);
			return nullptr;
		}
	}
}
