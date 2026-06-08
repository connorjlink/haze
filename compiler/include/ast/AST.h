#ifndef HAZE_AST_H
#define HAZE_AST_H

#include <utility/Sum.h>
#include <utility/Typing.h>

// Haze AST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define BASE_AST_METHODS(X, handlet) \
	X(format,   std::string) \
	X(generate, void) \
	X(evaluate, handlet) \
	X(optimize, handlet)

	// expose a strict polymorphic interface for AST nodes, including external data like IR
	template<typename AnchorT, typename HandleT>
	using BaseASTMethods = AllButLastT
	<
#define X(name, handlet) METHOD_TUPLE_ENTRY(name, handlet)
		BASE_AST_METHODS(X, HandleT)
#undef X
		void
	>;


#define AST_METHODS(X, handlet) \
	BASE_AST_METHODS(X, handlet) \
	X(get_type, TypeHandle)

	// further constrain pure AST types like expression and statements for type checking purposes
	template<typename AnchorT, typename HandleT>
	using ASTMethods = AllButLastT
	<
#define X(name, handlet) METHOD_TUPLE_ENTRY(name, handlet)
		AST_METHODS(X, HandleT)
#undef X
		void
	>;


	template<typename SumMemberT, typename SumStorageT, typename MethodsT>
	concept IsASTNode = SumTuple<SumMemberT, SumStorageT, MethodsT>;
}

#endif