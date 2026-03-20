#ifndef HAZE_TYPESPECIFIER_H
#define HAZE_TYPESPECIFIER_H

#include <toolchain/models/Token.h>
#include <type/IntTypeType.h>

// Haze TypeSpecifier.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeSpecifier
	{
		BYTE,
		WORD,
		DWORD,
		STRING,
		STRUCT,
		NVR, // "void"
	};

	static const std::unordered_map<TokenType, TypeSpecifier> _type_specifier_token_map
	{
		{ TokenType::S8, TypeSpecifier::BYTE },
		{ TokenType::U8, TypeSpecifier::BYTE },
		{ TokenType::S16, TypeSpecifier::WORD },
		{ TokenType::U16, TypeSpecifier::WORD },
		{ TokenType::S32, TypeSpecifier::DWORD },
		{ TokenType::U32, TypeSpecifier::DWORD },
		{ TokenType::STRING, TypeSpecifier::STRING },
		{ TokenType::STRUCT, TypeSpecifier::STRUCT },
		{ TokenType::NVR, TypeSpecifier::NVR },
	};

	static const std::unordered_map<TypeSpecifier, IntTypeType> _type_specifier_int_map
	{
		{ TypeSpecifier::BYTE, IntTypeType::INT8 },
		{ TypeSpecifier::WORD, IntTypeType::INT16 },
		{ TypeSpecifier::DWORD, IntTypeType::INT32 },
	};

	static const std::unordered_map<TypeSpecifier, std::string> _type_specifier_map
	{
		{ TypeSpecifier::BYTE, "byte" },
		{ TypeSpecifier::WORD, "word" },
		{ TypeSpecifier::DWORD, "dword" },
		{ TypeSpecifier::STRING, "string" },
		{ TypeSpecifier::STRUCT, "struct" },
		{ TypeSpecifier::NVR, "nvr" },
	};
}

#endif 
