#ifndef HAZE_TYPESIGNEDNESS_H
#define HAZE_TYPESIGNEDNESS_H

// Haze TypeSignedness.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeSignedness
	{
		SIGNED,
		UNSIGNED,
	};

	static const std::unordered_map<TokenType, TypeSignedness> _type_signedness_token_map
	{
		{ TokenType::SIGNED, TypeSignedness::SIGNED },
		{ TokenType::UNSIGNED, TypeSignedness::UNSIGNED },
	};

	static const std::unordered_map<TypeSignedness, std::string> _type_signedness_map
	{
		{ TypeSignedness::SIGNED, "signed" },
		{ TypeSignedness::UNSIGNED, "unsigned" },
	};
}

#endif 
