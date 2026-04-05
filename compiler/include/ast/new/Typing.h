#ifndef HAZE_TYPING_H
#define HAZE_TYPING_H

// Haze Typing.h
// (c) Connor J. Link. All Rights Reserved. 

// TODO: remove
#include <tuple>
#include <vector>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <format>

namespace hz
{
    // meta type traits
    template<typename T, typename Tuple>
    struct TypeIndex;

    template<typename T, typename... Types>
    struct TypeIndex<T, std::tuple<T, Types...>> 
        : public std::integral_constant<std::size_t, 0> 
    {
    };

    template<typename T, typename U, typename... Types>
    struct TypeIndex<T, std::tuple<U, Types...>> 
        : public std::integral_constant<std::size_t, 1 + TypeIndex<T, std::tuple<Types...>>::value> 
    {
    };

    template<typename T, typename Tuple>
    using TypeIndexV = TypeIndex<T, Tuple>::value;


    // function type traits
    template<auto MethodPointer, typename Signature>
    struct Method;

    template<auto MethodPointer, typename R, typename... Args>
    struct Method<MethodPointer, R(Args...)>
    {
        static constexpr auto pointer = MethodPointer;
        using ReturnType = R;
    };
}

#endif
