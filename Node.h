#ifndef HAZE_NODE_H
#define HAZE_NODE_H

#include <string>

namespace hz
{
    //Required to avoid dependency between Instruction and Allocation
    class Allocation;

    class Node
    {
    protected:
        enum class Type
        {
            FUNCTION,
            STATEMENT,
            EXPRESSION,
            INSTRUCTION,
        };

    public:
        virtual Node::Type ntype() const = 0;
        virtual std::string string() const = 0;
        virtual Node* copy() const = 0;
        virtual void generate(Allocation*) = 0;
        virtual Node* optimize() = 0;
    };
}

#endif //HAZE_NODE_H
