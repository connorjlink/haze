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
	X(AUTO,     auto) \
	X(REGISTER, register) \
	X(STATIC,   static) \
	X(EXTERN,   extern)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, StorageClass)

	DEFINE_ENUM(STORAGE_CLASSES, StorageClass, storage struct)

#undef AUTOENUM_ROUTER


	constexpr StorageClass token_to_storage_class(TokenKind token)
	{
		switch (token)
		{
#define X(enumerator, name) case TokenKind::enumerator: return StorageClass::enumerator;
			STORAGE_CLASSES(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"unknown storage struct `{}`", token), NULL_TOKEN);

		return StorageClass::AUTO;
	}
}

#endif
