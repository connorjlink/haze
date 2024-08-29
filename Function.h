#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include "Token.h"
#include "Statement.h"
#include "Expression.h"
#include "Context.h"

#include <string>
#include <vector>
#include <unordered_map>

// Haze Function.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_FUNCTION(x) static_cast<Function*>(x)

namespace hz
{
    enum class ReturnType
    {
	    BYTE,
    	NVR,
    };

    static const std::unordered_map<TokenType, ReturnType> _type_map
    {
        { TokenType::BYTE, ReturnType::BYTE },
        { TokenType::NVR, ReturnType::NVR },
    };

    class Function : public Node
    {
    public:
        std::string name;
        ReturnType return_type;
        std::vector<Expression*> arguments;
        Statement* body;

    public:
        Function(std::string name, ReturnType return_type, std::vector<Expression*>&& arguments, Statement* body, Token token)
            : Node{ token }, name{ std::move(name) }, return_type{ return_type }, arguments{ std::move(arguments) }, body{ body }
        {
        }

    public:
        virtual NodeType ntype() const final override;
        virtual std::string string() const final override;
        virtual Function* copy() const final override;
        virtual void generate(Allocation*) final override;
        virtual Function* optimize() final override;
        virtual Node* evaluate(Context*) const final override;
    };
}

#endif //HAZE_FUNCTION_H
