#include "Statement.h"

namespace hz
{
    Statement::Type CompoundStatement::stype() const
    {
        return Statement::Type::COMPOUND;
    }

    Segment CompoundStatement::generate(Allocation*)
    {
        Segment result{};

        for (auto substatement : substatements)
        {
            auto allocation = Allocator::allocate_static();
            result.append(substatement->generate(allocation));
        }

        return result;
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

    Segment VariableStatement::generate(Allocation* received_allocation)
    {
        Segment result{};

        allocation = received_allocation;
        if (value)
        {
            result.append(value->generate(allocation));
        }

        return result;
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

    Segment ReturnStatement::generate(Allocation* received_allocation)
    {
        Segment result{};

        allocation = received_allocation;
        if (value)
        {
            result.append(value->generate(allocation));
        }

        return result;
    }

    Statement* ReturnStatement::optimize()
    {
        //TODO: implement this
        return nullptr;
    }
}