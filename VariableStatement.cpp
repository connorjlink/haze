#include "VariableStatement.h"

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

    void VariableStatement::generate(Allocation* received_allocation)
    {
        if (value)
        {
	        value->generate(received_allocation);
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