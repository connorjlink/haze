#include "CompoundStatement.h"
#include "Allocator.h"

#include <format>

namespace hz
{
    StatementType CompoundStatement::stype() const
    {
        return StatementType::COMPOUND;
    }

    std::string CompoundStatement::string() const
    {
        std::string substatements_string = "";

        for (auto substatement : substatements)
        {
            substatements_string += std::format("\t{}\n", substatement->string());
        }

        return std::format("compound statement \n[\n{}]\n", substatements_string);
    }

    CompoundStatement* CompoundStatement::copy() const
    {
        return new CompoundStatement{ *this };
    }

    void CompoundStatement::generate(Allocation*)
    {
        for (auto substatement : substatements)
        {
            substatement->generate();
        }
    }

    Statement* CompoundStatement::optimize()
    {
        std::vector<Statement*> substatements_optimized{};

        bool did_optimize = false;

        for (auto substatement : substatements)
        {
            if (auto substatement_optimized = substatement->optimize())
            {
                substatements_optimized.emplace_back(AS_STATEMENT(substatement_optimized));
                did_optimize = true;
            }

            else
            {
                substatements_optimized.emplace_back(substatement);
            }
        }

        if (!did_optimize)
        {
            return nullptr;
        }

        return new CompoundStatement{ std::move(substatements_optimized) };
    }

    Node* CompoundStatement::evaluate(Context* context) const
    {
        for (auto& substatement : substatements)
        {
            // TODO: is it valid to throw away the result of a statement evaluation?
            DISCARD substatement->evaluate(context);
        }

        return nullptr;
    }
}