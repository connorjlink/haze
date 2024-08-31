#ifndef HAZE_NODE_H
#define HAZE_NODE_H

#include "NodeType.h"
#include "Token.h"

// Haze Node.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    // Required to avoid dependency between Instruction and Allocation
    class Allocation;

    // Required to avoid dependency between Node and Context
    class Context;

    class Node
    {
    public:
        Token _token;

    public:
        Node(Token token)
            : _token{ token }
        {
        }

    public:
        virtual NodeType ntype() const = 0;
        virtual Node* copy() const = 0;
        virtual void generate(Allocation* = nullptr) = 0;
        virtual Node* optimize() = 0;

        // use this to store execution information in the interpreter
        // this could be extended later to do compile time stuff in the compiler
        virtual Node* evaluate(Context*) const = 0;
    };
}

#endif
