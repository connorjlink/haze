#ifndef HAZE_TYPEKIND_H
#define HAZE_TYPEKIND_H

#include <utility/AutoEnum.h>

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


#define AUTOENUM_ROUTER(X, enumerator, precedence, type, name) X(enumerator, type, name, TypeKind)

	DEFINE_ENUM(TYPE_KINDS, TypeKind, type kind)

#undef AUTOENUM_ROUTER
}

#endif
