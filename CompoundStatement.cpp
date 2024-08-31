#include "CompoundStatement.h"
#include "Allocator.h"
#include "Utility.h"

namespace hz
{
    StatementType CompoundStatement::stype() const
    {
        return StatementType::COMPOUND;
    }

    CompoundStatement* CompoundStatement::copy() const
    {
        return new CompoundStatement{ *this };
    }

    void CompoundStatement::generate(Allocation*)
    {
        for (auto substatement : substatements)
        {
            if (substatement != nullptr)
            {
                substatement->generate();
            }
        }
    }

    Statement* CompoundStatement::optimize()
    {
        std::vector<Statement*> substatements_optimized{};

        bool did_optimize = false;

        for (auto substatement : substatements)
        {
            if (substatement != nullptr)
            {
                if (auto substatement_optimized = substatement->optimize();
                    substatement_optimized != nullptr)
                {
                    substatements_optimized.emplace_back(AS_STATEMENT(substatement_optimized));
                    did_optimize = true;
                }

                else
                {
                    substatements_optimized.emplace_back(substatement);
                }
            }
        }

        if (!did_optimize)
        {
            return nullptr;
        }

        return new CompoundStatement{ std::move(substatements_optimized), _token };
    }

    Node* CompoundStatement::evaluate(Context* context) const
    {
        for (auto& substatement : substatements)
        {
            substatement->evaluate(context);
        }

        return nullptr;
    }
}