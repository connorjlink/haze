#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include <vector>
#include <string_view>

#include "Statement.h"

namespace hz
{
    using Argument = std::string_view;

    struct Function
    {
        std::string_view name;
        std::vector<Argument> arguments;
        Statement* body;
    };
}

#endif //HAZE_FUNCTION_H
