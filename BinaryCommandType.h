#ifndef HAZE_BINARYCOMMANDTYPE_H
#define HAZE_BINARYCOMMANDTYPE_H

namespace hz
{
	enum class BinaryCommandType
	{
		ADD,
		SUBTRACT,
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
