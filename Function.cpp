#include "Function.h"

#include <fmt/format.h>

namespace hz
{
    Node::Type Function::ntype() const
    {
        return Node::Type::FUNCTION;
    }


    Function::Type RuntimeFunction::ftype() const
    {
        return Function::Type::RUNTIME;
    }

    Function::Type CompiletimeFunction::ftype() const
    {
        return Function::Type::COMPILETIME;
    }


    std::string RuntimeFunction::string() const
    {
        return fmt::format("run-time function ({}({}))", name, fmt::join(arguments, ", "));
    }

    std::string CompiletimeFunction::string() const
    {
        return fmt::format("compile-time function ({}({}))", name, fmt::join(arguments, ", "));
    }


    RuntimeFunction* RuntimeFunction::copy() const
    {
        return new RuntimeFunction{ *this };
    }

    CompiletimeFunction* CompiletimeFunction::copy() const
    {
        return new CompiletimeFunction{ *this };
    }


    Segment RuntimeFunction::generate(Allocation* allocation)
    {
        return {};
    }

    Segment CompiletimeFunction::generate(Allocation* allocation)
    {
        //TODO: determine the best way of netting the proper return value
        return {};
    }


    Function* RuntimeFunction::optimize()
    {
        if (auto body_optimized = body->optimize(); body_optimized)
        {
            return new RuntimeFunction{ name, arguments, body_optimized };
        }

        return nullptr;
    }

    Function* CompiletimeFunction::optimize()
    {
        //No need to optimize what is already a compile time function
        return nullptr;
    }
}
