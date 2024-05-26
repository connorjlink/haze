#include "Statement.h"
#include "Allocator.h"

#include <format>

namespace hz
{
    Node::Type Statement::ntype() const
    {
        return Node::Type::STATEMENT;
    }
}