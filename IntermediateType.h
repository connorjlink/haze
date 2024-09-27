#ifndef HAZE_INTERMEDIATETYPE_H
#define HAZE_INTERMEDIATETYPE_H

// Haze IntermediateType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_BRANCH_COMMAND(x) static_cast<BranchCommand*>(x)
#define AS_BRANCH_LABEL_COMMAND(x) static_cast<BranchLabelCommand*>(x)
#define AS_CALL_FUNCTION_COMMAND(x) static_cast<CallFunctionCommand*>(x)

namespace hz
{
	enum class IntermediateType
	{
		BINARY,
		BRANCH,
		BRANCH_LABEL,
		ENTER_SCOPE,
		LEAVE_SCOPE,
		LOCAL_VARIABLE,
		GLOBAL_VARIABLE,
		HEAP_READ,
		HEAP_WRITE,
		STACK_READ,
		STACK_WRITE,
		INCREMENT,
		DECREMENT,
		BOOL,
		COPY,
		MAKE_IMMEDIATE,
		MAKE_ARGUMENT,
		TAKE_ARGUMENT,
		VOID_RETURN,
		VALUE_RETURN,
		MAKE_MESSAGE,
		PRINT_MESSAGE,
		PRINT_NUMBER,
		EXIT_PROGRAM,
		INLINE_ASSEMBLY,
	};
}

#endif 
