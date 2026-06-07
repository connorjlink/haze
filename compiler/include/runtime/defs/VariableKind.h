#ifndef HAZE_VARIABLEKIND_H
#define HAZE_VARIABLEKIND_H

#include <utility/AutoEnum.h>

// Haze VariableKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define VARIABLE_KINDS(X) \
	X(VOID, VoidVariable, void) \
	X(INT, IntVariable, int) \
	X(FLOAT, FloatVariable, float) \
	X(STRUCT_OR_UNION, StructOrUnionVariable, struct or union) \
	X(ENUM, EnumVariable, enum)


#define ENUM_MEMBER(enumerator, type, name) enumerator,
#define SWITCH_CASE(enumerator, type, name) case VariableKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, type, name) Mapping{ #name, VariableKind::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, VARIABLE_KINDS, VariableKind, variable kind)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
