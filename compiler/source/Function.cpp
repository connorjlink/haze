import std;

#include <allocator/Allocation.h>
#include <ast/Function.h>
#include <ast/IdentifierExpression.h>
#include <ast/ArgumentExpression.h>
#include <ast/CompoundStatement.h>
#include <ast/ReturnStatement.h>
#include <error/CommonErrors.h>
#include <runtime/Evaluator.h>
#include <toolchain/Parser.h>
#include <toolchain/CompilerParser.h>
#include <x86/X86Register.h>

// Haze Function.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Function::Function(const std::string& name, Type* return_type, const std::vector<Expression*>& arguments, Statement* body, const Token& token)
		: Node{ token }, name{ name }, return_type{ return_type }, arguments{ arguments }, body{ body }
	{
	}

	NodeType Function::ntype() const
	{
		return NodeType::FUNCTION;
	}

	void Function::generate(Allocation*)
	{
		REQUIRE_SAFE(Generator)->begin_function(name);

		// ensure each function implicitly has its own scope block
		if (body->stype() != StatementType::COMPOUND)
		{
			body = new CompoundStatement{ { body }, body->_token };
		}

		const auto arity = arguments.size();

		auto function_symbol = USE_UNSAFE(SymbolDatabase)->reference_function(name, _token);

		REQUIRE_SAFE(Generator)->begin_scope();

		// map stack space for the formals
		for (auto argument : arguments)
		{
			auto argument_expression = AS_ARGUMENT_EXPRESSION(argument);
			REQUIRE_SAFE(RuntimeAllocator)->define_local(argument_expression->identifier->name);
		}


		body->generate();

#pragma message("TODO: store function labels in a common dependency to avoid this hack")
		if (REQUIRE_SAFE(Parser)->ptype() == ParserType::COMPILER)
		{
			auto compiler_parser = AS_COMPILER_PARSER(REQUIRE_SAFE(Parser).get());

			const auto& end_function_label = compiler_parser->_function_label_map.at(name);
			REQUIRE_SAFE(Generator)->branch_label(end_function_label);

			REQUIRE_SAFE(Generator)->end_scope();
		}
		else
		{
			CommonErrors::invalid_parser_type(REQUIRE_SAFE(Parser)->ptype(), _token);
			return;
		}
		

		// NOTE: opting to generate the function epilogue for every single return statement
		// in case there was no return statement, a default return value of 0 is provided
#pragma message("TODO: figure out if there was not a return statement")
		if constexpr (false)
		{
			USE_SAFE(ErrorReporter)->post_warning(std::format(
				"implicit return generated for function `{}`", name), body->_token);
			
			auto implicit_return = new ReturnStatement{ name, return_value, new ObserverAllocation{ EAX }, NULL_TOKEN};
			// NOTE: generating a statement, so no need to pass an allocation
			implicit_return->generate(nullptr);
		}
	}

	Function* Function::optimize()
	{
		if (auto body_optimized = body->optimize())
		{
			return new Function{ name, return_type, arguments, AS_STATEMENT(body_optimized), _token };
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

			for (auto i = 0uz; i < arguments.size(); i++)
			{
				const auto& argument_name = AS_IDENTIFIER_EXPRESSION(arguments[i])->name;
				context->define_variable(argument_name, node_to_variable(arguments_evaluated[i]));
			}

			body->evaluate(context);
			return nullptr;
		}
	}
}
