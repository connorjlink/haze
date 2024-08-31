#include "ExitStatement.h"

// Haze ExitStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType ExitStatement::stype() const
	{
		return StatementType::EXIT;
	}

}