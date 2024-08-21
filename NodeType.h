#ifndef HAZE_NODETYPE_H
#define HAZE_NODETYPE_H

#define AS_NODE(x) static_cast<Node*>(x)

namespace hz
{
    enum class NodeType
    {
        FUNCTION,
        STATEMENT,
        EXPRESSION,
        COMMAND,
        INTRINSIC,
    };
}

#endif 
