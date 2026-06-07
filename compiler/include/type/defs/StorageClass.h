#ifndef HAZE_STORAGECLASS_H
#define HAZE_STORAGECLASS_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/models/Token.h>
#include <utility/AutoEnum.h>

// Haze StorageClass.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define STORAGE_CLASSES(X) \
	X(AUTO, auto) \
	X(REGISTER, register) \
	X(STATIC, static) \
	X(EXTERN, extern)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case StorageClass::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, StorageClass::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, STORAGE_CLASSES, StorageClass, storage class)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER


	constexpr StorageClass token_to_storage_class(TokenKind token)
	{
		switch (token)
		{
#define X(enumerator, name) case TokenKind::enumerator: return StorageClass::enumerator;
			STORAGE_CLASSES(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"unknown storage class `{}`", token), NULL_TOKEN);

		return StorageClass::AUTO;
	}
}

#endif
