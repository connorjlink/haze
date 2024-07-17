#include "StatementDeclarator.h"

#include <format>

namespace hz
{
	Declarator::Type StatementDeclarator::dtype() const
	{
		return Declarator::Type::STATEMENT;
	}
}