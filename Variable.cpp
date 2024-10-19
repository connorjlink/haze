import std;

#include "Variable.h"
#include "ErrorReporter.h"

// Haze Variable.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	VariableType UnsignedByteVariable::vtype() const
	{
		return VariableType::UBYTE;
	}

	std::string UnsignedByteVariable::format() const
	{
		return std::format("{}", value);
	}


	VariableType SignedByteVariable::vtype() const
	{
		return VariableType::SBYTE;
	}

	std::string SignedByteVariable::format() const
	{
		return std::format("{}", value);
	}


	VariableType UnsignedWordVariable::vtype() const
	{
		return VariableType::UWORD;
	}

	std::string UnsignedWordVariable::format() const
	{
		return std::format("{}", value);
	}


	VariableType SignedWordVariable::vtype() const
	{
		return VariableType::SWORD;
	}

	std::string SignedWordVariable::format() const
	{
		return std::format("{}", value);
	}


	VariableType UnsignedDoubleWordVariable::vtype() const
	{
		return VariableType::UDWORD;
	}

	std::string UnsignedDoubleWordVariable::format() const
	{
		return std::format("{}", value);
	}


	VariableType SignedDoubleWordVariable::vtype() const
	{
		return VariableType::SDWORD;
	}

	std::string SignedDoubleWordVariable::format() const
	{
		return std::format("{}", value);
	}


	VariableType UnsignedQuadWordVariable::vtype() const
	{
		return VariableType::UQWORD;
	}

	std::string UnsignedQuadWordVariable::format() const
	{
		return std::format("{}", value);
	}


	VariableType SignedQuadWordVariable::vtype() const
	{
		return VariableType::SQWORD;
	}

	std::string SignedQuadWordVariable::format() const
	{
		return std::format("{}", value);
	}


	VariableType StringVariable::vtype() const
	{
		return VariableType::STRING;
	}

	std::string StringVariable::format() const
	{
		return value;
	}


	VariableType StructVariable::vtype() const
	{
		return VariableType::STRUCT;
	}

	std::string StructVariable::format() const
	{
		_error_reporter->post_error(std::format("undefined string conversion for user struct type `{}`", tag), NULL_TOKEN);
		return std::format("<struct::{}>", tag);
	}
}