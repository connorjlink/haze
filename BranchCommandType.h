#ifndef HAZE_BRANCHCOMMANDTYPE_H
#define HAZE_BRANCHCOMMANDTYPE_H

// Haze BranchCommandType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class BranchCommandType
	{
		CALL_FUNCTION,
		IF_NOT_ZERO,
		IF_ZERO,
		GOTO,
		VOID_RETURN,
		VALUE_RETURN,
	};
}

#endif 
