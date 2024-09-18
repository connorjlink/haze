﻿#ifndef HAZE_INTERMEDIATETYPE_H
#define HAZE_INTERMEDIATETYPE_H

namespace hz
{
	enum class IntermediateType
	{
		ENTER_SCOPE,
		LEAVE_SCOPE,
		LOCAL_VARIABLE,
		GLOBAL_VARIABLE,
		MEMORY_READ,
		MEMORY_WRITE,
		ADD,
		SUBTRACT,
		BITOR,
		BITAND,
		BITXOR,
		INCREMENT,
		DECREMENT,
		COPY,
		MAKE_IMMEDIATE,
		MAKE_ARGUMENT,
		TAKE_ARGUMENT,
		CALL_FUNCTION,
		VOID_RETURN,
		VALUE_RETURN,
		IF_NOT_ZERO,
		GOTO,
		MAKE_MESSAGE,
		PRINT_MESSAGE,
		EXIT_PROGRAM,
		INLINE_ASSEMBLY,
	};
}

#endif 
