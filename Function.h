#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include "Node.h"
#include "TypeSpecifier.h"

#include <string>
#include <vector>

// Haze Function.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class Expression;
    class Statement;

    class Function : public Node
    {
    public:
        std::string name;
        TypeSpecifier return_type;
        std::vector<Expression*> arguments;
        Statement* body;

    public:
        Function(std::string, TypeSpecifier, std::vector<Expression*>&&, Statement*, Token);

    public:
        virtual NodeType ntype() const final override;
        virtual Function* copy() const final override;
        virtual void generate(Allocation*) final override;
        virtual Function* optimize() final override;
        virtual Node* evaluate(Context*) const final override;
    };
}

#endif
