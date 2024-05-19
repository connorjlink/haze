#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include <vector>
#include <string_view>

#include "Statement.h"

#define AS_RUNTIME_FUNCTION(x) return static_cast<RuntimeFunction*>(x)
#define AS_COMPILETIME_FUNCTION(x) return static_cast<CompileTimeFunction*>(x)

namespace hz
{
    class Function : public Node
    {
    public:
        std::string_view name;
        std::vector<Expression*> arguments;
        Statement* body;

    public:
        const decltype(arguments)& get_arguments() const
        {
            return arguments;
        }

        const decltype(body) get_body() const
        {
            return body;
        }

    public:
        Function(std::string_view name, std::vector<Expression*> arguments, Statement* body)
            : name(name), arguments(arguments), body(body)
        {
        }

    public:
        virtual Node::Type ntype() const final override;
        virtual std::string string() const final override;
        virtual Function* copy() const final override;
        virtual void generate(Allocation*) final override;
        virtual Function* optimize() final override;
    };
}

#endif //HAZE_FUNCTION_H
