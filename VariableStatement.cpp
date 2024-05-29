#include "VariableStatement.h"
#include "Allocator.h"
#include "Parser.h"

#include <format>

namespace hz
{
	Statement::Type VariableStatement::stype() const
    {
        return Statement::Type::VARIABLE;
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
        allocation = allocator->allocate_dynamic();
        AS_VARIABLE_SYMBOL(parser->reference_symbol(Symbol::Type::VARIABLE, name))->allocation = allocation;

        if (value)
        {
            auto temp_allocation = allocator->allocate_static();
            {
            	value->generate(temp_allocation);
				allocation->copy(temp_allocation);   
            }
            delete temp_allocation;
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
}
