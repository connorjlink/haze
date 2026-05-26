#ifndef HAZE_TYPECHECK_H
#define HAZE_TYPECHECK_H

// Haze TypeCheck.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Type;
	class Expression;

	TypeHandle resolve_type(ExpressionHandle);
	bool check_type(ExpressionHandle, TypeHandle);
}

#endif
