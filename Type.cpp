import std;

#include "Type.h"
#include "Symbol.h"
#include "Parser.h"

// Haze Type.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	TypeType IntType::ttype() const
	{
		return TypeType::INT;
	}

	std::uint16_t IntType::size() const
	{
		using enum IntTypeType;
		switch (int_type)
		{
			case INT8: return 1;
			case INT16: return 2;
			case INT32: return 4;
			case INT64: return 8;
		}
	}


	TypeType StructType::ttype() const
	{
		return TypeType::STRUCT;
	}

	std::uint16_t StructType::size() const
	{
		auto symbol = _parser->reference_symbol(SymbolType::STRUCT, tag, NULL_TOKEN);
		auto struct_symbol = AS_STRUCT_SYMBOL(symbol);

		const auto member_count = struct_symbol->members.size();
		const auto member_size = sizeof(platform_address_size);

#pragma message("TODO: compute the real size of the struct based on each member's size and offset!")

		return member_count * member_size;
	}


	TypeType StringType::ttype() const
	{
		return TypeType::STRING;
	}

	std::uint16_t StringType::size() const
	{
		const auto address_size = sizeof(platform_address_size);
		return address_size;
	}
}