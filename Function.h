#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include <vector>
#include <string>

#include "Statement.h"
#include "Expression.h"

namespace hz
{
    enum class ReturnType
    {
	    BYTE,
    	NVR,
    };

    class Function : public Node
    {
    public:
        std::string name;
        ReturnType return_type;
        std::vector<Expression*> arguments;
        Statement* body;

    public:
        Function(std::string name, ReturnType return_type, std::vector<Expression*>&& arguments, Statement* body)
            : name(std::move(name)), return_type(return_type), arguments(std::move(arguments)), body(body)
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
