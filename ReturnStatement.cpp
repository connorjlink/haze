#include "ReturnStatement.h"
#include "Function.h"
#include "Parser.h"
#include "Allocation.h"
#include "Generator.h"
#include "Evaluator.h"
#include "IntegerLiteralExpression.h"
#include "Symbol.h"
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
		/*
			_generator->journal_leave();

			using enum TypeSpecifier;
			switch (return_type)
			{
				case NVR:
				{
					_generator->make_return();
				} break;

				case BYTE:
				{
					AutoStackAllocation temp{};
					_generator->make_immediate(temp.source()->read(), 0);
					_generator->make_return(temp.source()->read());
				} break;
			}
		*/
#pragma message("TODO: redo the return statement generation to ensure journal_leave() is called for an epilogue")

		if (value == nullptr)
		{
			if (AS_FUNCTION_SYMBOL(_parser->reference_symbol(SymbolType::FUNCTION, enclosing_function, _token))->return_type != TypeSpecifier::NVR)
			{
				_error_reporter->post_error("no return value was specified for a non-`nvr` function", _token);
				return;
			}

			_generator->make_return();
		}



		AutoStackAllocation temp{};
		value->generate(temp.source());
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
