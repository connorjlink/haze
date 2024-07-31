#include "Statement.h"
#include "Allocator.h"

namespace hz
{
    NodeType Statement::ntype() const
    {
        return NodeType::STATEMENT;
    }
}