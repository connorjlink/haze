#include "FunctionCallExpression.h"
#include "Parser.h"
#include "Allocator.h"
#include "Allocation.h"

#include <format>

namespace hz
{
    Expression::Type FunctionCallExpression::etype() const
    {
        return Expression::Type::FUNCTION_CALL;
    }

    std::string FunctionCallExpression::string() const
    {
        return std::format("function call ({})", name);
    }

    FunctionCallExpression* FunctionCallExpression::copy() const
    {
        return new FunctionCallExpression{ *this };
    }

    void FunctionCallExpression::generate(Allocation* allocation)
    {
        auto symbol = parser->reference_symbol(Symbol::Type::FUNCTION, name, true);

		auto temp_allocation = allocator->allocate_static(allocation->read());
        {
            for (auto argument : arguments)
		    {
		    	argument->generate(temp_allocation);
		    	generator->push(temp_allocation->read());
            }

            //Placeholder call address before we hot-patch in the correct target after linking
		    generator->call(0xCCCC);

            //return value comes off stack
            generator->pull(allocation->read());
        }
        delete temp_allocation;
    }

    Expression* FunctionCallExpression::optimize()
    {
        //TODO: implement determination if a function is constexpr
        //in that case, we can fold it down to a single integer literal node
        //for now, we don't have that functionality so just terminate further optimization of this node
        return nullptr;
    }
}