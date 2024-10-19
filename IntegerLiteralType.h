#ifndef HAZE_INTEGERLITERALTYPE_H
#define HAZE_INTEGERLITERALTYPE_H

// Haze IntegerLiteralType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_UNSIGNED_BYTE(x) static_cast<UnsignedByteIntegerLiteral&>(x)
#define AS_SIGNED_BYTE(x) static_cast<SignedByteIntegerLiteral&>(x)
#define AS_UNSIGNED_WORD(x) static_cast<UnsignedWordIntegerLiteral&>(x)
#define AS_SIGNED_WORD(x) static_cast<SignedWordIntegerLiteral&>(x)
#define AS_UNSIGNED_DOUBLE_WORD(x) static_cast<UnsignedDoubleWordIntegerLiteral&>(x)
#define AS_SIGNED_DOUBLE_WORD(x) static_cast<SignedDoubleWordIntegerLiteral&>(x)
#define AS_UNSIGNED_QUAD_WORD(x) static_cast<UnsignedQuadWordIntegerLiteral&>(x)
#define AS_SIGNED_QUAD_WORD(x) static_cast<SignedQuadWordIntegerLiteral&>(x)

#define AS_UNSIGNED_BYTE_INTEGER_LITERAL(x) static_cast<UnsignedByteIntegerLiteral*>(x)
#define AS_SIGNED_BYTE_INTEGER_LITERAL(x) static_cast<SignedByteIntegerLiteral*>(x)
#define AS_UNSIGNED_WORD_INTEGER_LITERAL(x) static_cast<UnsignedWordIntegerLiteral*>(x)
#define AS_SIGNED_WORD_INTEGER_LITERAL(x) static_cast<SignedWordIntegerLiteral*>(x)
#define AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(x) static_cast<UnsignedDoubleWordIntegerLiteral*>(x)
#define AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(x) static_cast<SignedDoubleWordIntegerLiteral*>(x)
#define AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(x) static_cast<UnsignedQuadWordIntegerLiteral*>(x)
#define AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(x) static_cast<SignedQuadWordIntegerLiteral*>(x)

namespace hz
{
	enum class IntegerLiteralType
	{
		UBYTE, SBYTE,
		UWORD, SWORD,
		UDWORD, SDWORD,
		UQWORD, SQWORD,
	};

	static const std::unordered_map<IntegerLiteralType, std::string> _integer_literal_type_map
	{
		{ IntegerLiteralType::UBYTE, "unsigned byte" },
		{ IntegerLiteralType::SBYTE, "signed byte" },
		{ IntegerLiteralType::UWORD, "unsigned word" },
		{ IntegerLiteralType::SWORD, "signed word" },
		{ IntegerLiteralType::UDWORD, "unsigned double word" },
		{ IntegerLiteralType::SDWORD, "signed double word" },
		{ IntegerLiteralType::UQWORD, "unsigned quad word" },
		{ IntegerLiteralType::SQWORD, "signed quad word" },
	};
}

#endif
