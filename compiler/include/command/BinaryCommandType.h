#ifndef HAZE_BINARYCOMMANDTYPE_H
#define HAZE_BINARYCOMMANDTYPE_H

// Haze BinaryCommandType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class BinaryCommandType
	{
		ADD,
		SUBTRACT,
		MULTIPLY,
		BITOR,
		BITAND,
		BITXOR,
		BITLSHIFT,
		BITRSHIFT,
		EQUALITY,
		INEQUALITY,
		LESS,
		GREATER,
	};
}

#endif 
