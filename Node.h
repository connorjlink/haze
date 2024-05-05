#ifndef HAZE_NODE_H
#define HAZE_NODE_H
#include "Segment.h"

namespace hz
{
    class Parser;
    class Allocation;

    class Node
    {
    protected:
        static inline Parser* parser;

    protected:
        enum class Type
        {
            FUNCTION,
            STATEMENT,
            EXPRESSION,
        };

    public:
        virtual Node::Type ntype() const = 0;
        virtual std::string string() const = 0;
        virtual Node* copy() const = 0;
        virtual void generate(Allocation*) = 0;
        virtual Node* optimize() = 0;
    };

    extern Parser* parser;
}

#endif //HAZE_NODE_H
