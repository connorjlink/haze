#ifndef HAZE_TYPESIGNEDNESS_H
#define HAZE_TYPESIGNEDNESS_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/models/Token.h>
#include <utility/AutoEnum.h>

// Haze TypeSignedness.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#include <type/defs/TypeSignedness.x>

#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case TypeSignedness::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, TYPE_SIGNEDNESSES, TypeSignedness, type signedness)

	constexpr TypeSignedness to_type_signedness(TokenKind token_type)
	{
		switch (token_type)
		{
#define X(enumerator, name) case TokenKind::enumerator: return TypeSignedness::name;
			TYPE_SIGNEDNESSES(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid type signedness token '{}'", token_type), NULL_TOKEN);

		return TypeSignedness::SIGNED;
	}
}

#endif 
