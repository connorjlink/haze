#ifndef HAZE_TYPESTORAGE_H
#define HAZE_TYPESTORAGE_H

// Haze TypeStorage.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeStorage
	{
		VALUE,
		PTR,
	};

	static const std::unordered_map<TypeStorage, std::string> _type_storage_map
	{
		{ TypeStorage::VALUE, "value" },
		{ TypeStorage::PTR, "ptr" },
	};
}

#endif