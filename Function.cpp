#include "Function.h"
#include "Generator.h"
#include "Allocator.h"
#include "IdentifierExpression.h"
#include "Utility.h"
#include "Log.h"

#include <format>
#include "Evaluator.h"


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
		// TODO: support argument inputs here!

		_generator->begin_function(name);
		body->generate();
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
