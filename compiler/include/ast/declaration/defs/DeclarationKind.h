#ifndef HAZE_DECLARATIONKIND_H
#define	HAZE_DECLARATIONKIND_H

#include <utility/AutoEnum.h>

// Haze DeclarationKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define DECLARATION_KINDS(X) \
	X(FUNCTION,        FunctionDeclaration,      function) \
	X(VARIABLE,        VariableDeclaration,      variable) \
	X(STRUCT_OR_UNION, StructOrUnionDeclaration, struct or union) \
	X(ENUM,            EnumDeclaration,          enum) \
	X(TYPEDEF,         TypedefDeclaration,       typedef)


#define AUTOENUM_ROUTER(X, enumerator, type, name) X(enumerator, type, name, DeclarationKind)

	DEFINE_ENUM(DECLARATION_KINDS, DeclarationKind, declaration kind)

#undef AUTOENUM_ROUTER
}

#endif
