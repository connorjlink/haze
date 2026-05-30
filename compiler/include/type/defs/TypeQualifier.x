#ifndef HAZE_TYPEQUALIFIER_X
#define HAZE_TYPEQUALIFIER_X

// Haze TypeQualifier.x
// (c) Connor J. Link. All Rights Reserved.

#define TYPE_QUALIFIERS(X) \
	X(NONE, none, 0) \
	X(CONST, const, 1 << 0) \
	X(VOLATILE, volatile, 1 << 1) \
	X(RESTRICT, restrict, 1 << 2)

#endif
