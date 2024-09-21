import std;

#include "ReturnStatement.h"
#include "Function.h"
#include "Parser.h"
#include "Allocation.h"
#include "Generator.h"
#include "Evaluator.h"
#include "IntegerLiteralExpression.h"
#include "Symbol.h"
#include "X86Builder.h"
#include "ErrorReporter.h"

// Haze ReturnStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType ReturnStatement::stype() const
	{
		return StatementType::RETURN;
	}

	ReturnStatement* ReturnStatement::copy() const
	{
		return new ReturnStatement{ *this };
	}

	void ReturnStatement::generate(Allocation*)
	{
		// when value==nullptr, expect no return value ONLY from nvr function
		if (value == nullptr)
		{
			if (AS_FUNCTION_SYMBOL(_parser->reference_symbol(SymbolType::FUNCTION, enclosing_function, _token))->return_type != TypeSpecifier::NVR)
			{
				_error_reporter->post_error("no return value was specified for a non-`nvr` function", _token);
				return;
			}

			// NOTE: no parameters return 
			// simply destroy the stack frame and return
			_generator->end_scope();
			_generator->make_return();
		}

		// first generate the return value
		AutoStackAllocation temp{};
		value->generate(temp.source());

		// then destroy the stack frame and return the value
		_generator->end_scope();
		_generator->make_return(temp.source()->read());
	}

	Statement* ReturnStatement::optimize()
	{
		if (auto value_optimized = value->optimize())
		{
			return new ReturnStatement{ enclosing_function, AS_EXPRESSION(value_optimized), allocation, _token };
		}

		return nullptr;
	}

	Node* ReturnStatement::evaluate(Context* context) const
	{
		context->push_return(harvest(value->evaluate(context)));
		return nullptr;
	}
}
