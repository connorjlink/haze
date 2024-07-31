#ifndef HAZE_NODETYPE_H
#define HAZE_NODETYPE_H

namespace hz
{
    enum class NodeType
    {
        FUNCTION,
        STATEMENT,
        EXPRESSION,
        COMMAND,
        DECLARATOR,
        INTRINSIC,
    };
}

#endif 
