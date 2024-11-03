#ifndef HAZE_TYPETYPE_H
#define HAZE_TYPETYPE_H

// Haze TypeType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_INT_TYPE(x) static_cast<IntType*>(x)
#define AS_STRUCT_TYPE(x) static_cast<StructType*>(x)
#define AS_STRING_TYPE(x) static_cast<StringType*>(x)
#define AS_VOID_TYPE(x) static_assert("Cannot cast to `void` type" && false)

namespace hz
{
	enum class TypeType
	{
		INT,
		STRUCT,
		STRING,
		VOID,
	};

	static const std::unordered_map<TypeType, std::string> _type_type_map
	{
		{ TypeType::INT, "int" },
		{ TypeType::STRUCT, "struct" },
		{ TypeType::STRING, "string" },
		{ TypeType::VOID, "void" },
	};
}

#endif
