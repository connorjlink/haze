#ifndef HAZE_AST_H
#define HAZE_AST_H

#include <ast/new/Sum.h>
#include <ast/new/Typing.h>

// Haze AST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#pragma message("TODO: figure out a better way to enforce a single ground truth anchor function signature set")

    // expose a strict polymorphic interface for AST nodes
    template<typename AnchorT>
    using ASTMethods = std::tuple
    <
        Method<&AnchorT::ttype, decltype(&AnchorT::ttype)>,
        Method<&AnchorT::format, decltype(&AnchorT::format)>,
        Method<&AnchorT::evaluate, decltype(&AnchorT::evaluate)>,
        Method<&AnchorT::optimize, decltype(&AnchorT::optimize)>,
        Method<&AnchorT::check_types, decltype(&AnchorT::check_types)>
    >;
    
    template<typename SumMemberT, typename SumStorageT, typename MethodsT>
    concept ASTNode = SumTuple<SumMemberT, SumStorageT, MethodsT>;
}

#endif