#ifndef HAZE_TYPEKIND_H
#define HAZE_TYPEKIND_H

// Haze TypeKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeKind
	{
		VOID,
		INT,
		FLOAT,
		STRUCT_OR_UNION,
		ENUM,
		TYPEDEF_NAME,
		POINTER,
		ARRAY,
		FUNCTION,
	};

	static const std::unordered_map<TypeKind, std::string_view> _type_kind_map
	{
		{ TypeKind::VOID, "void" },
		{ TypeKind::INT, "int" },
		{ TypeKind::FLOAT, "float" },
		{ TypeKind::STRUCT_OR_UNION, "struct or union" },
		{ TypeKind::ENUM, "enum" },
		{ TypeKind::TYPEDEF_NAME, "typedef name" },
		{ TypeKind::POINTER, "pointer" },
		{ TypeKind::ARRAY, "array" },
		{ TypeKind::FUNCTION, "function" },
	};
}

#endif
