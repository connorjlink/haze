#include "Statement.h"
#include "Allocator.h"

namespace hz
{
    Statement::Type CompoundStatement::stype() const
    {
        return Statement::Type::COMPOUND;
    }

    void CompoundStatement::generate(Allocation*)
    {
        for (auto substatement : substatements)
        {
            auto allocation = allocator->allocate_static();
            substatement->generate(allocation);
        }
    }

    Statement* CompoundStatement::optimize()
    {
        //TODO: fix
        /*
        auto result = new CompoundStatement{};

        for (auto substatement : substatements)
        {
            if (auto optimized = substatement->optimize())
            {
                result->substatements.emplace_back(optimized);
            }
        return result;*/

        //TODO: implement this
        return nullptr;
    }


    Statement::Type VariableStatement::stype() const
    {
        return Statement::Type::VARIABLE;
    }

    void VariableStatement::generate(Allocation* received_allocation)
    {
        /*allocation = received_allocation;
        if (value)
        {
            value->generate(allocation);
        }*/
        //TODO: is the above wrong? why does it set `allocation`
        if (value)
        {
            value->generate(received_allocation);
        }
    }

    Statement* VariableStatement::optimize()
    {
        //TODO: implement this
        return nullptr;
    }


    Statement::Type ReturnStatement::stype() const
    {
        return Statement::Type::RETURN;
    }

    void ReturnStatement::generate(Allocation* received_allocation)
    {
        /*
        allocation = received_allocation;
        if (value)
        {
            result.append(value->generate(allocation));
        }*/
        //TODO: is the above wrong? why does it set `allocation`
        if (value)
        {
            value->generate(received_allocation);
        }
    }

    Statement* ReturnStatement::optimize()
    {
        //TODO: implement this
        return nullptr;
    }
}