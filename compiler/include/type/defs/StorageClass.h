#ifndef HAZE_STORAGECLASS_H
#define HAZE_STORAGECLASS_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/models/Token.h>

// Haze StorageClass.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class StorageClass
	{
#define X(enumerator, name) enumerator,
#include <type/defs/StorageClass.def>
#undef X
	};

	constexpr std::string_view to_string(StorageClass storage)
	{
		switch (storage)
		{
#define X(enumerator, name) case StorageClass::enumerator: return #name;
#include <type/defs/StorageClass.def>
#undef X
		}

		return "<unknown storage class>";
	}

	constexpr StorageClass token_to_storage_class(TokenType token)
	{
		switch (token)
		{
#define X(enumerator, name, value) case TokenType::enumerator: return StorageClass::enumerator;
#include <type/defs/StorageClass.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"unknown storage class `{}`", token), NULL_TOKEN);

		return StorageClass::AUTO;
	}
}

#endif
