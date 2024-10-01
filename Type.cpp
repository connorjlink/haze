import std;

#include "Type.h"

// Haze Type.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	TypeType IntType::ttype() const
	{
		return TypeType::INT;
	}

	TypeType StructType::ttype() const
	{
		return TypeType::STRUCT;
	}

	TypeType StringType::ttype() const
	{
		return TypeType::STRING;
	}
}