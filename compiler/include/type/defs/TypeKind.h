#ifndef HAZE_TYPEKIND_H
#define HAZE_TYPEKIND_H

// Haze TypeKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define TYPE_KINDS(X) \
	X(VOID, VoidType, void) \
	X(INT, IntType, int) \
	X(FLOAT, FloatType, float) \
	X(STRUCT_OR_UNION, StructOrUnionType, struct-or-union) \
	X(ENUM, EnumType, enum) \
	X(TYPEDEF_NAME, TypedefNameType, typedef-name) \
	X(POINTER, PointerType, pointer) \
	X(ARRAY, ArrayType, array) \
	X(FUNCTION, FunctionType, function)


#define ENUM_MEMBER(enumerator, type, name) enumerator,
#define SWITCH_CASE(enumerator, type, name) case TypeKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, TYPE_KINDS, TypeKind, type kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
