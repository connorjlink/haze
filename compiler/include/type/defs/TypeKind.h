#ifndef HAZE_TYPEKIND_H
#define HAZE_TYPEKIND_H

// Haze TypeKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define TYPE_KINDS(X) \
	X(VOID,            LEAF,     VoidType,          void) \
	X(INT,             LEAF,     IntType,           int) \
	X(FLOAT,           LEAF,     FloatType,         float) \
	X(STRUCT_OR_UNION, LEAF,     StructOrUnionType, struct-or-union) \
	X(ENUM,            LEAF,     EnumType,          enum) \
	X(TYPEDEF_NAME,    LEAF,     TypedefNameType,   typedef-name) \
	X(POINTER,         POINTER,  PointerType,       pointer) \
	X(ARRAY,           ARRAY,    ArrayType,         array) \
	X(FUNCTION,        FUNCTION, FunctionType,      function)


#define ENUM_MEMBER(enumerator, precedence, type, name) enumerator,
#define SWITCH_CASE(enumerator, precedence, type, name) case TypeKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, TYPE_KINDS, TypeKind, type kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
