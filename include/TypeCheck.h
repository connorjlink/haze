#ifndef HAZE_TYPECHECK_H
#define HAZE_TYPECHECK_H

// Haze TypeCheck.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Type;
	class Expression;

	Type* resolve_type(Expression*);
	bool check_type(Expression*, Type*);
}

#endif
