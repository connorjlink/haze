#ifndef HAZE_AST_H
#define HAZE_AST_H

#include <ast/new/Sum.h>
#include <ast/new/Typing.h>

// Haze AST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    // expose a strict polymorphic interface for AST nodes
    template<typename SumStorageT>
    using ASTMethods = std::tuple
    <
        Method<&(typename SumStorageT::Anchor)::ttype, TagType()>,
        Method<&(typename SumStorageT::Anchor)::format, std::string()>,
        Method<&(typename SumStorageT::Anchor)::evaluate, SumHandle<SumStorageT, typename SumStorageT::Type>(const SumStorageT&)>,
        Method<&(typename SumStorageT::Anchor)::optimize, SumHandle<SumStorageT, typename SumStorageT::Type>(const SumStorageT&)>, // will return a handle
        Method<&(typename SumStorageT::Anchor)::check_types, bool(const SumStorageT&)>
    >;
    
    template<typename SumMemberT, typename SumStorageT, typename Anchor>
    concept ASTNode = SumTuple<SumMemberT, SumStorageT, ASTMethods<SumStorageT>>;
}

#endif
