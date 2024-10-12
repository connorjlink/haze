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
		SBYTE, UBYTE,

		SWORD, UWORD,

		SDWORD, UDWORD,

		SQWORD, UQWORD,

		STRING,

		STRUCT,
	};
}

#endif
