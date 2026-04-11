#ifndef HAZE_STORAGECLASS_H
#define HAZE_STORAGECLASS_H

// Haze StorageClass.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class StorageClass 
	{
		AUTO,
		REGISTER,
		STATIC,
		EXTERN,
		TYPEDEF,
	};

	static const std::unordered_map<TokenType, StorageClass> _storage_class_token_map
	{
		{ TokenType::AUTO, StorageClass::AUTO },
		{ TokenType::REGISTER, StorageClass::REGISTER },
		{ TokenType::STATIC, StorageClass::STATIC },
		{ TokenType::EXTERN, StorageClass::EXTERN },
		{ TokenType::TYPEDEF, StorageClass::TYPEDEF },
	};

	static const std::unordered_map<StorageClass, std::string_view> _storage_class_map
	{
		{ StorageClass::AUTO, "auto" },
		{ StorageClass::REGISTER, "register" },
		{ StorageClass::STATIC, "static" },
		{ StorageClass::EXTERN, "extern" },
		{ StorageClass::TYPEDEF, "typedef" },
	};    
}

#endif
