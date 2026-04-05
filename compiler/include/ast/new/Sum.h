#ifndef HAZE_SUM_H
#define HAZE_SUM_H

// Haze Sum.h
// (c) Connor J. Link. All Rights Reserved.

// TODO: REMOVE
#include <tuple>
#include <vector>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <format>

namespace hz
{
    // sum vector storage definitions: 255 subtypes, 4GiO nodes per subtype
    using TagType = std::uint8_t;
    using IndexType = std::uint32_t;

    template<template<typename> typename Index, typename This>
    struct InjectTagType 
    {
    public:
        constexpr TagType ttype() const
        {
            return Index<This>::value;
        }
    };

    template<typename SumT>
    struct InjectStorage
    {
    public:
        using Storage = typename SumT::Storage;

        template<typename T>
        using SumReference = SumReference<T, Storage>;
    };


    // dynamic dispatch generator for sum families
    template<typename Fn, typename SumStorageT, typename Tuple, std::size_t... Is>
    consteval auto make_dispatch_table_impl(std::index_sequence<Is...>)
    {
        return std::array
        {
            [](const SumStorageT& sum, IndexType index) -> typename Fn::ReturnType
            {
                using T = typename std::tuple_element_t<Is, Tuple>;
                const auto& vector = sum.storage.template get<T>();
                return (vector[index].*Method::pointer)(sum);
            }...
        };
    }

    template<typename Fn, typename SumStorageT, typename Tuple>
    consteval auto make_dispatch_table()
    {
        return make_dispatch_table_impl<Fn, SumStorageT, Tuple>(
            std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }


    // type-checked discriminated linear sum storage for statically known sum families
    template<typename... Ts>
    struct SumStorage 
    {   
    public:
        static constexpr std::size_t size = sizeof...(Ts);

    public:
        std::tuple<std::vector<Ts>...> storage;

    public:
        template<typename T>
        std::vector<T>& get()
        {
            constexpr auto I = TypeIndex<T, std::tuple<Ts...>>::value;
            return std::get<I>(storage);
        }

        template<typename T>
        const std::vector<T>& get() const
        {
            constexpr auto I = TypeIndex<T, std::tuple<Ts...>>::value;
            return std::get<I>(storage);
        }

    public:
        template<typename T, typename... Args>
        T& emplace_back(Args&&... args)
        {
            return get<T>().emplace_back(std::forward<Args>(args)...);
        }

        template<typename T>
        void push_back(T&& value)
        {
            get<T>().push_back(std::forward<T>(value));
        }
    };


    // node dispatch type traits
    template<typename T, typename Sum, typename Fn>
    concept ImplementsMethod = requires(const T& node, const Sum& sum)
    {
        { (node.*Fn::pointer)(sum) } -> std::same_as<typename Fn::ReturnType>;
    };

    template<typename T, typename Sum, typename MethodsT, std::size_t... Is>
    concept SumTupleImpl = (ImplementsMethod<T, Sum, std::tuple_element_t<Is, MethodsT>> && ...);

    template<typename T, typename Sum, typename MethodsT>
    concept SumTuple = SumTupleImpl<T, Sum, MethodsT, std::make_index_sequence<std::tuple_size_v<MethodsT>>{}>;


    // sum type and dispatch family
    template<template<typename, typename> typename MethodsT, typename... Ts>
        requires (sizeof...(Ts) > std::numeric_limits<TagType>::min() && sizeof...(Ts) <= std::numeric_limits<TagType>::max())
    struct Sum
    {
    public:
        template<std::size_t I>
        using TypeAt = std::tuple_element_t<I, Type>;

    public:
        using Storage = SumStorage<Ts...>;
        using Type = typename Storage::Type;
        using Anchor = typename Storage::Anchor;

    public:
        using Methods = MethodsT<Sum, Anchor>;

    public:
        template<typename T, typename Sum, typename Anchor>
        concept Element = SumTuple<T, Sum, Methods<Sum, Anchor>>;

    public:
        struct Storage : public SumStorage<Ts...>
        {
        };
    };

    template<typename... Ts>
    struct SumTypeList
    {
        using Type = std::tuple<Ts...>;
        using Anchor = std::tuple_element_t<0, Type>;

        template<typename T>
        using Index = TypeIndexV<T, Type>;
    };

    template<template<typename> typename MethodsT, typename TypeList>
    struct MakeSum;

    template<template<typename> typename MethodsT, typename... Ts>
    struct MakeSum<MethodsT, SumTypeList<Ts...>>
    {
        using Type = Sum<MethodsT, Ts...>;
    };



    // dynamic dispatch wrapper type for sum families
    template<typename SumStorageT>
    struct SumHandle
    {
    public:
        using Type = typename SumStorageT::Type;
        using Anchor = typename SumStorageT::Anchor;

    public:
        const SumStorageT& sum_storage;
        IndexType index;
        TagType tag;
        bool is_valid;

    public:
        operator bool() const
        {
            return is_valid;
        }

    public:
        template<typename Method>
        decltype(auto) call() const
        {
            static constinit auto table =
                make_dispatch_table<Method, SumStorageT, Type>();

            return table[tag](sum_storage, index);
        }

    public:
        #define SUM_HANDLE_METHODS(X) \
            X(print) \
            X(evaluate) \
            X(optimize) \
            X(check_types)

        #define SUM_HANDLE_METHOD(name) \
            decltype(auto) name() const \
            { \
                return call<Method<&Anchor::name, decltype(&Anchor::name)>>(); \
            }

        SUM_HANDLE_METHODS(SUM_HANDLE_METHOD)

        #undef SUM_HANDLE_METHOD
        #undef SUM_HANDLE_METHODS
    };

    // entry point into sum dynamic dispatch
    template<typename SumMemberT, typename SumStorageT>
    SumHandle<SumStorageT> make_handle(const SumStorageT& sum_storage, IndexType index)
    {
        return SumHandle<SumStorageT>{ sum_storage, index, TypeIndex<SumMemberT, typename SumStorageT::Type>::value, true };
    }
#define MAKE_HANDLE(sum_storage, index) make_handle<decltype(*this), std::decay_t<decltype(sum_storage)>, typename std::decay_t<decltype(sum_storage)>::Type>(sum_storage, index)

    template<typename SumMemberT, typename SumStorageT>
    SumHandle<SumStorageT> make_invalid_handle(const SumStorageT& sum_storage)
    {
        return SumHandle<SumStorageT>{ sum_storage, 0, 0, false };
    }
#define MAKE_INVALID_HANDLE(sum_storage) make_invalid_handle<decltype(*this), std::decay_t<decltype(sum_storage)>, typename std::decay_t<decltype(sum_storage)>::Type>(sum_storage)


    // a type-specifc slim wrapper under the sun handle
    template<typename T, typename SumStorageT>
    struct SumReference
    {
    public:
        IndexType index;

    public:
        const T& get(const SumStorageT& sum_storage) const
        {
            return sum_storage.template get<T>()[index];
        }

        T& get(SumStorageT& sum_storage) const
        {
            return sum_storage.template get<T>()[index];
        }

    public:
        SumHandle<SumStorageT> erase(const SumStorageT& sum_storage) const
        {
            return make_handle<T, SumStorageT>(sum_storage, index);
        }
    };

}

#endif
