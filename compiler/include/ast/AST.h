#ifndef HAZE_AST_H
#define HAZE_AST_H

#include <utility/Sum.h>
#include <utility/Typing.h>

// Haze AST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define BASE_AST_METHODS(X) \
	X(format) \
	X(generate) \
	X(evaluate) \
	X(optimize)

	// expose a strict polymorphic interface for AST nodes, including external data like IR
	template<typename AnchorT>
	using BaseASTMethods = AllButLastT
	<
#define X(name) Method<&AnchorT::name, decltype(&AnchorT::name)>,
		BASE_AST_METHODS(X)
#undef X
		void
	>;


#define AST_METHODS(X) \
	BASE_AST_METHODS(X) \
	X(format) \

	// further constrain pure AST types like expression and statements for type checking purposes
	template<typename AnchorT>
	using ASTMethods = AllButLastT
	<
#define X(name) Method<&AnchorT::name, decltype(&AnchorT::name)>,
		AST_METHODS(X)
#undef X
		void
	>;


	template<typename SumMemberT, typename SumStorageT, typename MethodsT>
	concept ASTNode = SumTuple<SumMemberT, SumStorageT, MethodsT>;
}

#endif