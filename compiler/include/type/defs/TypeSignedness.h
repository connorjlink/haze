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
#define TYPE_SIGNEDNESSES(X) \
	X(SIGNED,   signed) \
	X(UNSIGNED, unsigned)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, TypeSignedness)

	DEFINE_ENUM(TYPE_SIGNEDNESSES, TypeSignedness, type signedness)

#undef AUTOENUM_ROUTER


	constexpr TypeSignedness token_to_type_signedness(TokenKind token_type)
	{
		switch (token_type)
		{
#define X(enumerator, name) case TokenKind::enumerator: return TypeSignedness::enumerator;
			TYPE_SIGNEDNESSES(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid type signedness token `{}`", token_type), NULL_TOKEN);

		return TypeSignedness::SIGNED;
	}
}

#endif 
