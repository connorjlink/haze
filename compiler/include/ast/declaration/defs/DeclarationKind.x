#ifndef HAZE_DECLARATIONKIND_X
#define HAZE_DECLARATIONKIND_X

// Haze DeclarationKind.x
// (c) Connor J. Link. All Rights Reserved.

#define DECLARATION_KINDS(X) \
	X(FUNCTION, FunctionDeclaration, function) \
	X(VARIABLE, VariableDeclaration, variable) \
	X(STRUCT_OR_UNION, StructOrUnionDeclaration, struct or union) \
	X(ENUM, EnumDeclaration, enum) \
	X(TYPEDEF, TypedefDeclaration, typedef)

#endif
