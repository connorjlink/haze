import std;

#include "CompoundStatement.h"
#include "Allocator.h"

// Haze CompoundStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

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
        _generator->begin_scope();

        for (auto substatement : substatements)
        {
            if (substatement != nullptr)
            {
                substatement->generate();
            }
        }

        // NOTE: old method
        // per updated standards, it is now considered UB not to return from a non-nvr function
        // this means there is no reason to emit a duplicate stack-frame-destroy command
        // _generator->end_scope();
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