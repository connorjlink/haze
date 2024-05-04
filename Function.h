#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include <vector>
#include <string_view>

#include "Statement.h"

namespace hz
{
    class Function : public Node
    {
    public:
        enum class Type
        {
            RUNTIME,
            COMPILETIME,
        };

    public:
        std::string_view name;
        std::vector<std::string_view> arguments;
        Statement* body;

    public:
        const std::vector<std::string_view>& get_arguments() const
        {
            return arguments;
        }

        const Statement* const get_body() const
        {
            return body;
        }

    public:
        virtual Function::Type ftype() const = 0;
        virtual Node::Type ntype() const final override;
    };

    class RuntimeFunction : public Function
    {
    public:
        virtual Function::Type ftype() const final override;
        virtual Segment generate(Allocation*) final override;
        virtual Function* optimize() final override;
    };

    class CompiletimeFunction : public Function
    {
    public:
        virtual Function::Type ftype() const final override;
        virtual Segment generate(Allocation*) final override;
        virtual Function* optimize() final override;
    };
}

#endif //HAZE_FUNCTION_H
