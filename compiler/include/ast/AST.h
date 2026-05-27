#ifndef HAZE_AST_H
#define HAZE_AST_H

#include <utility/Sum.h>
#include <utility/Typing.h>

// Haze AST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// expose a strict polymorphic interface for AST nodes, including external data like IR
	template<typename AnchorT>
	using BaseASTMethods = std::tuple
	<
		Method<&AnchorT::format, decltype(&AnchorT::format)>,
		Method<&AnchorT::generate, decltype(&AnchorT::generate)>,
		Method<&AnchorT::evaluate, decltype(&AnchorT::evaluate)>,
		Method<&AnchorT::optimize, decltype(&AnchorT::optimize)>
	>;

	// further constrain pure AST types like expression and statements for type checking purposes
	template<typename AnchorT>
	using ASTMethods = decltype(std::tuple_cat(
		std::declval<BaseASTMethods<AnchorT>>(),
		std::declval<std::tuple<Method<&AnchorT::get_type, decltype(&AnchorT::get_type)>>>()));

	template<typename SumMemberT, typename SumStorageT, typename MethodsT>
	concept ASTNode = SumTuple<SumMemberT, SumStorageT, MethodsT>;
}

#endif