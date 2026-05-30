#ifndef HAZE_TYPEKIND_X
#define HAZE_TYPEKIND_X

// Haze TypeKind.x
// (c) Connor J. Link. All Rights Reserved.

#define TYPE_KINDS(X) \
	X(VOID, void, VoidType) \
	X(INT, int, IntType) \
	X(FLOAT, float, FloatType) \
	X(STRUCT_OR_UNION, struct-or-union, StructOrUnionType) \
	X(ENUM, enum, EnumType) \
	X(TYPEDEF_NAME, typedef-name, TypedefNameType) \
	X(POINTER, pointer, PointerType) \
	X(ARRAY, array, ArrayType) \
	X(FUNCTION, function, FunctionType)

#endif
