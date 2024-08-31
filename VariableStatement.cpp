#include "VariableStatement.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Evaluator.h"

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
        // Make some space on the heap and notify the parser
        allocation = _allocator->allocate_dynamic();
        AS_VARIABLE_SYMBOL(_parser->reference_symbol(SymbolType::VARIABLE, name, NULL_TOKEN))->allocation = allocation;

        if (value)
        {
            ManagedStaticAllocation temp{};
            value->generate(temp.allocation);
        	allocation->copy(temp.allocation); 
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
