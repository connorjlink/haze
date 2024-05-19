#include "Statement.h"
#include "Allocator.h"

#include <format>

namespace hz
{
    Node::Type Statement::ntype() const
    {
        return Node::Type::STATEMENT;
    }


    Statement::Type CompoundStatement::stype() const
    {
        return Statement::Type::COMPOUND;
    }

    Statement::Type VariableStatement::stype() const
    {
        return Statement::Type::VARIABLE;
    }

    Statement::Type ReturnStatement::stype() const
    {
        return Statement::Type::RETURN;
    }


    std::string CompoundStatement::string() const
    {
        //TODO: figure out how to print all substatements here
        return std::format("compound statement (...)");
    }

    std::string VariableStatement::string() const
    {
        return std::format("variable declaration ({} = {})", name, value ? value->string() : "undefined");
    }

    std::string ReturnStatement::string() const
    {
        return std::format("return ({})", value->string());
    }


    CompoundStatement* CompoundStatement::copy() const
    {
        return new CompoundStatement{ *this };
    }

    VariableStatement* VariableStatement::copy() const
    {
        return new VariableStatement{ *this };
    }

    ReturnStatement* ReturnStatement::copy() const
    {
        return new ReturnStatement{ *this };
    }


    void CompoundStatement::generate(Allocation*)
    {
        for (auto substatement : substatements)
        {
            auto allocation = allocator->allocate_static();
            substatement->generate(allocation);
        }
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

    Statement* VariableStatement::optimize()
    {
        //TODO: implement this
        return nullptr;
    }

    Statement* ReturnStatement::optimize()
    {
        //TODO: implement this
        return nullptr;
    }
}