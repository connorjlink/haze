#include "Statement.h"
#include "Allocator.h"

namespace hz
{
    Node::Type Statement::ntype() const
    {
        return Node::Type::STATEMENT;
    }
}