#include "Function.h"

namespace hz
{
    Function::Type RuntimeFunction::type()
    {
        return Function::Type::RUNTIME;
    }

    Segment RuntimeFunction::generate()
    {

    }

    Function* RuntimeFunction::optimize()
    {
        return new RuntimeFunction{ name, arguments, body->optimize() };
    }




    Function::Type CompiletimeFunction::type()
    {
        return Function::Type::COMPILETIME;
    }

    Segment CompiletimeFunction::generate()
    {
        //TODO: determine the best way of netting the proper return value
        return {};
    }

    Function* CompiletimeFunction::optimize()
    {
        //No need to optimize what is already a compile time function
        return new CompiletimeFunction{ *this };
    }
}