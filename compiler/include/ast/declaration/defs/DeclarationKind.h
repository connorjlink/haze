#ifndef HAZE_DECLARATIONKIND_H
#define	HAZE_DECLARATIONKIND_H

// Haze DeclarationKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define DECLARATION_KINDS(X) \
	X(FUNCTION, FunctionDeclaration, function) \
	X(VARIABLE, VariableDeclaration, variable) \
	X(STRUCT_OR_UNION, StructOrUnionDeclaration, struct or union) \
	X(ENUM, EnumDeclaration, enum) \
	X(TYPEDEF, TypedefDeclaration, typedef)

#define ENUM_MEMBER(enumerator, type, name) enumerator,
#define SWITCH_CASE(enumerator, type, name) case DeclarationKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, type, name) Mapping{ #name, DeclarationKind::enumerator },

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, DECLARATION_KINDS, DeclarationKind, declaration kind)

#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
