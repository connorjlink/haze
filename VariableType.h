#ifndef HAZE_VARIABLETYPE_H
#define HAZE_VARIABLETYPE_H

// Haze VariableType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_SBYTE_VARIABLE(x) static_cast<SignedByteVariable*>(x)
#define AS_UBYTE_VARIABLE(x) static_cast<UnsignedByteVariable*>(x)
#define AS_SWORD_VARIABLE(x) static_cast<SignedWordVariable*>(x)
#define AS_UWORD_VARIABLE(x) static_cast<UnsignedWordVariable*>(x)
#define AS_SDWORD_VARIABLE(x) static_cast<SignedDoubleWordVariable*>(x)
#define AS_UDWORD_VARIABLE(x) static_cast<UnsignedDoubleWordVariable*>(x)
#define AS_SQWORD_VARIABLE(x) static_cast<SignedQuadWordVariable*>(x)
#define AS_UQWORD_VARIABLE(x) static_cast<UnsignedQuadWordVariable*>(x)
#define AS_STRING_VARIABLE(x) static_cast<StringVariable*>(x)
#define AS_STRUCT_VARIABLE(x) static_cast<StructVariable*>(x)

namespace hz
{
	enum class VariableType
	{
		UBYTE, SBYTE,

		UWORD, SWORD,

		UDWORD, SDWORD,

		UQWORD, SQWORD,

		STRING,

		STRUCT,
	};

	static const std::unordered_map<VariableType, std::string> _variable_type_map
	{
		{ VariableType::UBYTE, "unsigned byte" },
		{ VariableType::SBYTE, "signed byte" },
		{ VariableType::UWORD, "unsigned word" },
		{ VariableType::SWORD, "signed word" },
		{ VariableType::UDWORD, "unsigned double word" },
		{ VariableType::SDWORD, "signed double word" },
		{ VariableType::UQWORD, "unsigned quad word" },
		{ VariableType::SQWORD, "signed quad word" },
		{ VariableType::STRING, "string" },
		{ VariableType::STRUCT, "struct" },
	};
}

#endif
