#ifndef HAZE_TYPESIGNEDNESS_H
#define HAZE_TYPESIGNEDNESS_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/models/Token.h>

// Haze TypeSignedness.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeSignedness
	{
#define X(enumerator, name) enumerator,
#include <type/defs/TypeSignedness.x>
#undef X
	};

	constexpr std::string_view to_string(TypeSignedness signedness)
	{
		switch (signedness)
		{
#define X(enumerator, name) case TypeSignedness::enumerator: return #name;
#include <type/defs/TypeSignedness.x>
#undef X
		}

		return "<unknown type signedness>";
	}

	constexpr TypeSignedness to_type_signedness(TokenKind token_type)
	{
		switch (token_type)
		{
#define X(enumerator, name) case TokenKind::enumerator: return TypeSignedness::name;
#include <type/defs/TypeSignedness.x>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid type signedness token '{}'", token_type), NULL_TOKEN);

		return TypeSignedness::SIGNED;
	}
}

#endif 
