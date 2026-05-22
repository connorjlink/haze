#ifndef HAZE_TYPESIGNEDNESS_H
#define HAZE_TYPESIGNEDNESS_H

// Haze TypeSignedness.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeSignedness
	{
#define X(enumerator, name) enumerator,
#include <type/TypeSignedness.def>
#undef X
	};

	constexpr std::string_view to_string(TypeSignedness signedness)
	{
		switch (signedness)
		{
#define X(enumerator, name) case TypeSignedness::enumerator: return #name;
#include <type/TypeSignedness.def>
#undef X
		}

		return "<unknown type signedness>";
	}

	constexpr TypeSignedness to_type_signedness(TokenType token_type)
	{
		switch (token_type)
		{
#define X(enumerator, name) case TokenType::enumerator: return TypeSignedness::name;
#include <type/TypeSignedness.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid type signedness token '{}'", token_type), NULL_TOKEN);

		return TypeSignedness::SIGNED;
	}
}

template<>
struct std::formatter<hz::TypeSignedness>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}
	auto format(const hz::TypeSignedness& signedness, std::format_context& context) const
	{
		return std::format_to(context.out(), "{}", to_string(signedness));
	}
};

#endif 
