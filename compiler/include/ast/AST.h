#ifndef HAZE_AST_H
#define HAZE_AST_H

#include <utility/Sum.h>
#include <utility/Typing.h>

// Haze AST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define BASE_AST_METHODS(X, returntype) \
	X(format, std::string) \
	X(generate, void) \
	X(evaluate, returntype) \
	X(optimize, returntype)

	// expose a strict polymorphic interface for AST nodes, including external data like IR
	template<typename AnchorT, typename HandleT>
	using BaseASTMethods = AllButLastT
	<
#define X(name, returntype) Method<&AnchorT::name, decltype(&AnchorT::name)>,
		BASE_AST_METHODS(X, HandleT)
#undef X
		void
	>;


#define AST_METHODS(X, type) \
	BASE_AST_METHODS(X, type) \
	X(get_type, TypeHandle)

	// further constrain pure AST types like expression and statements for type checking purposes
	template<typename AnchorT, typename HandleT>
	using ASTMethods = AllButLastT
	<
#define X(name, returntype) Method<&AnchorT::name, decltype(&AnchorT::name)>,
		AST_METHODS(X, HandleT)
#undef X
		void
	>;


	template<typename SumMemberT, typename SumStorageT, typename MethodsT>
	concept ASTNode = SumTuple<SumMemberT, SumStorageT, MethodsT>;
}

#endif