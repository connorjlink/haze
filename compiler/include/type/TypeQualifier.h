#ifndef HAZE_TYPEQUALIFIER_H
#define HAZE_TYPEQUALIFIER_H

#include <toolchain/models/Token.h>

// Haze TypeQualifier.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#undef CONST

	enum class TypeQualifier
	{
#define X(enumerator, name, value) name = value,
#include <type/TypeQualifier.def>
#undef X
	};

	constexpr std::string_view format_type_qualifier(TypeQualifier qualifier)
	{
		using enum TypeQualifier;
		switch (qualifier)
		{
#define X(enumerator, name, value) case TypeQualifier::name: return #name;
#include <type/TypeQualifier.def>
#undef X
		}

		return "<unknown type qualifier>";
	}

	constexpr std::string to_string(TypeQualifier qualifier)
	{
		using enum TypeQualifier;

		return std::format("{}{}{}",
			(qualifier & CONST) ? "const " : "",
			(qualifier & VOLATILE) ? "volatile " : "",
			(qualifier & RESTRICT) ? "restrict " : "");
	}

	constexpr TypeQualifier operator|(TypeQualifier lhs, TypeQualifier rhs)
	{
		return static_cast<TypeQualifier>(
			static_cast<std::underlying_type_t<TypeQualifier>>(lhs) | 
			static_cast<std::underlying_type_t<TypeQualifier>>(rhs));
	}

	constexpr TypeQualifier operator|=(TypeQualifier& lhs, TypeQualifier rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	constexpr bool operator&(TypeQualifier lhs, TypeQualifier rhs)
	{
		return static_cast<bool>(
			static_cast<std::underlying_type_t<TypeQualifier>>(lhs) & 
			static_cast<std::underlying_type_t<TypeQualifier>>(rhs));
	}
}

#endif
