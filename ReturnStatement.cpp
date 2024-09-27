import std;

#include "ReturnStatement.h"
#include "Function.h"
#include "Parser.h"
#include "CompilerParser.h"
#include "Allocation.h"
#include "Generator.h"
#include "Evaluator.h"
#include "ArgumentExpression.h"
#include "IntegerLiteralExpression.h"
#include "Symbol.h"
#include "X86Builder.h"
#include "CommonErrors.h"
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
		auto symbol = _parser->reference_symbol(SymbolType::FUNCTION, enclosing_function, _token);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		if (_parser->ptype() == ParserType::COMPILER)
		{
			auto compiler_parser = AS_COMPILER_PARSER(_parser);
			const auto& end_function_label = compiler_parser->_function_label_map.at(enclosing_function);

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
				_generator->make_return(end_function_label);
			}

			// now, figure out how to return the value
			// NOTE: if there were arguments to the function, one of those allocations can be re-used

			//#pragma message("TODO: debug this to be able to share allocation with one of those from the function itself?")
			//if (function_symbol->arity() != 0)
			//{
			//	const auto argument_expression = AS_ARGUMENT_EXPRESSION(function_symbol->arguments[0]);
			//	const auto identifier = argument_expression->identifier;

			//	const auto symbol = _parser->reference_symbol(SymbolType::ARGUMENT, identifier->name, _token);
			//	const auto argument_symbol = AS_ARGUMENT_SYMBOL(symbol);

			//	const auto allocation = argument_symbol->allocation;

			//	value->generate(allocation);

			//	// then destroy the stack frame
			//	_generator->end_scope();
			//	_generator->make_return(allocation->read());

			//	return;
			//}

			if (_parser->ptype() == ParserType::COMPILER)
			{

				// if no arguments, make new allocation
				AutoStackAllocation temp{};
				// generate the return value
				value->generate(temp.source());

				_generator->make_return(end_function_label, temp.source()->read());
			}
		}

		else
		{
			CommonErrors::invalid_parser_type(_parser->ptype(), _token);
			return;
		}
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
