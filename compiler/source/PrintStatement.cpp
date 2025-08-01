import std;

#include <allocator/Allocation.h>
#include <ast/Expression.h>
#include <ast/PrintStatement.h>
#include <runtime/Evaluator.h>

// Haze PrintStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType PrintStatement::stype() const
	{
		return StatementType::PRINT;
	}

	PrintStatement* PrintStatement::copy() const
	{
		return new PrintStatement{ *this };
	}

	void PrintStatement::generate(Allocation*)
	{
		AutoStackAllocation temp{};
		message->generate(temp.source());

		using enum ExpressionType;
		switch (message->etype())
		{
			case STRING:
			{
				REQUIRE_SAFE(Generator)->print_message(temp.source()->read());
			} break;

			case INTEGER_LITERAL:
			{
				REQUIRE_SAFE(Generator)->print_number(temp.source()->read());
			} break;

			case IDENTIFIER:
			{
				REQUIRE_SAFE(Generator)->print_number(temp.source()->read());
			} break;

			default:
			{
				USE_SAFE(ErrorReporter)->post_error("unsupported compiler print expression type", message->_token);
			} break;
		}
	}

	Statement* PrintStatement::optimize()
	{
		if (auto message_optimized = message->optimize())
		{
			return new PrintStatement{ AS_EXPRESSION_NODE(message_optimized), _token };
		}
		
		return nullptr;
	}

	Node* PrintStatement::evaluate(Context* context) const
	{
		const auto value = node_to_variable(message->evaluate(context));
		context->print(format(value));
		return nullptr;
	}
}