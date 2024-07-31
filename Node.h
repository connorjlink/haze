#ifndef HAZE_NODE_H
#define HAZE_NODE_H

#include <string>
#include <vector>
#include <cstdint>

#include "Context.h"

#include "NodeType.h"

namespace hz
{
    //Required to avoid dependency between Instruction and Allocation
    class Allocation;

    class Node
    {
    public:
        virtual NodeType ntype() const = 0;
        virtual std::string string() const = 0;
        virtual Node* copy() const = 0;
        virtual void generate(Allocation* = nullptr) = 0;
        virtual Node* optimize() = 0;

        // we will use this to store execution information in the interpreter
        // this could be extended later to do compile time stuff in the compiler
        virtual Node* evaluate(Context*) const
        {
            return nullptr;
        }
    };
}

#endif //HAZE_NODE_H
