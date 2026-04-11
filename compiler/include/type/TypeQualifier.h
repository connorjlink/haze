#ifndef HAZE_TYPEQUALIFIER_H
#define HAZE_TYPEQUALIFIER_H

#include <toolchain/models/Token.h>

// Haze TypeQualifier.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeQualifier
	{
		NONE = 0,
		CONST = 1 << 0,
		VOLATILE = 1 << 1,
		RESTRICT = 1 << 2,
	};

	TypeQualifier operator|(TypeQualifier lhs, TypeQualifier rhs)
	{
		return static_cast<TypeQualifier>(
			static_cast<std::underlying_type_t<TypeQualifier>>(lhs) | 
			static_cast<std::underlying_type_t<TypeQualifier>>(rhs));
	}

	TypeQualifier operator|=(TypeQualifier& lhs, TypeQualifier rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	bool operator&(TypeQualifier lhs, TypeQualifier rhs)
	{
		return static_cast<bool>(
			static_cast<std::underlying_type_t<TypeQualifier>>(lhs) & 
			static_cast<std::underlying_type_t<TypeQualifier>>(rhs));
	}

	static const std::unordered_map<TokenType, TypeQualifier> _type_qualifier_token_map
	{
		{ TokenType::CONST, TypeQualifier::CONST },
		{ TokenType::VOLATILE, TypeQualifier::VOLATILE },
		{ TokenType::RESTRICT, TypeQualifier::RESTRICT },
	};

	static const std::unordered_map<TypeQualifier, std::string_view> _type_qualifier_map
	{
		{ TypeQualifier::CONST, "const" },
		{ TypeQualifier::VOLATILE, "volatile" },
		{ TypeQualifier::RESTRICT, "restrict" },
	};

	std::string format_type_qualifier(TypeQualifier qualifier)
	{
		using enum TypeQualifier;

		return std::format("{}{}{}",
			(qualifier & CONST) ? "const " : "",
			(qualifier & VOLATILE) ? "volatile " : "",
			(qualifier & RESTRICT) ? "restrict " : "");
	}
}

#endif
