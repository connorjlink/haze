#include "CompoundStatement.h"
#include "Allocator.h"

#include <format>

namespace hz
{
	Statement::Type CompoundStatement::stype() const
    {
        return Statement::Type::COMPOUND;
    }

    std::string CompoundStatement::string() const
    {
        std::string substatements_string = "";

        for (auto substatement : substatements)
        {
	        substatements_string += std::format("\t{}\n", substatement->string());
        }

        return std::format("compound statement \n[\n{}c]\n", substatements_string);
    }

    CompoundStatement* CompoundStatement::copy() const
    {
        return new CompoundStatement{ *this };
    }

    void CompoundStatement::generate(Allocation*)
    {
        auto allocation = allocator->allocate_static();

        for (auto substatement : substatements)
        {
            //TODO: determine if overwriting the allocation is actually bad?
            substatement->generate(allocation);
        }
    }

    Statement* CompoundStatement::optimize()
    {
        std::vector<Statement*> substatements_optimized{};

        for (auto substatement : substatements)
        {
	        if (auto substatement_optimized = substatement->optimize())
	        {
		        substatements_optimized.emplace_back(AS_STATEMENT(substatement_optimized));
	        }
        }

        return new CompoundStatement{ std::move(substatements_optimized) };
    }
}