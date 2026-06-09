#ifndef HAZE_VARIABLEKIND_H
#define HAZE_VARIABLEKIND_H

#include <utility/AutoEnum.h>

// Haze VariableKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define VARIABLE_KINDS(X) \
	X(VOID,            VoidVariable,          void) \
	X(INT,             IntVariable,           int) \
	X(FLOAT,           FloatVariable,         float) \
	X(STRUCT_OR_UNION, StructOrUnionVariable, struct or union) \
	X(ENUM,            EnumVariable,          enum)


#define AUTOENUM_ROUTER(X, enumerator, type, name) X(enumerator, type, name, VariableKind)

	DEFINE_ENUM(VARIABLE_KINDS, VariableKind, variable kind)

#undef AUTOENUM_ROUTER
}

#endif
