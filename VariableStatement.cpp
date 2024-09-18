#include "VariableStatement.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Evaluator.h"
#include "Symbol.h"

// Haze VariableStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType VariableStatement::stype() const
	{
		return StatementType::VARIABLE;
	}

	VariableStatement* VariableStatement::copy() const
	{
		return new VariableStatement{ *this };
	}

	void VariableStatement::generate(Allocation*)
	{
		// NOTE: old method
		// Make some space on the heap and notify the parser
		// also will need to support byte sizes other than 4
		/*allocation = new HeapAllocation{ 4 };
		AS_VARIABLE_SYMBOL(_parser->reference_symbol(SymbolType::VARIABLE, name, NULL_TOKEN))->allocation = allocation;

		if (value)
		{
			AutoStackAllocation temp{};
			value->generate(temp.source());
			temp.source()->copy_into(allocation);
		}*/


		if (value)
		{
			AutoStackAllocation temp{};
			value->generate(temp.source());
			_generator->make_local(name, temp.source()->read());
		}

		else
		{
			_generator->make_local(name);
		}
	}

	Statement* VariableStatement::optimize()
	{
		if (auto value_optimized = value->optimize())
		{
			return new VariableStatement{ type, name, AS_EXPRESSION(value_optimized), allocation, _token };
		}

		return nullptr;
	}

	Node* VariableStatement::evaluate(Context* context) const
	{
		if (value)
		{
			context->define_variable(name, harvest(value->evaluate(context)));
		}

		else
		{
			context->define_variable(name, variable_t{ 0u });
		}

		return nullptr;
	}
}
