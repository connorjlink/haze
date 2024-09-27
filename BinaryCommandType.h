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
		EQUALITY,
		INEQUALITY,
		LESS,
		GREATER,
	};
}

#endif 
