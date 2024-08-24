#include "VariableStatement.h"
#include "Allocator.h"
#include "Parser.h"
#include "Evaluator.h"

#include <format>

namespace hz
{
	StatementType VariableStatement::stype() const
    {
        return StatementType::VARIABLE;
    }

    std::string VariableStatement::string() const
    {
        return std::format("variable declaration ({} = {})", name, value ? value->string() : "undefined");
    }

    VariableStatement* VariableStatement::copy() const
    {
        return new VariableStatement{ *this };
    }

    void VariableStatement::generate(Allocation*)
    {
        //Make some space on the heap and notify the parser
        allocation = _allocator->allocate_dynamic();
        AS_VARIABLE_SYMBOL(_parser->reference_symbol(SymbolType::VARIABLE, name))->allocation = allocation;

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
	        return new VariableStatement{ name, AS_EXPRESSION(value_optimized), allocation };
        }

        return nullptr;
    }

    Node* VariableStatement::evaluate(Context* context) const
    {
        context->define_variable(name, harvest(value->evaluate(context)));
        return nullptr;
    }
}
