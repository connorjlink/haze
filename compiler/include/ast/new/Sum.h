#ifndef HAZE_SUM_H
#define HAZE_SUM_H

#include <error/ErrorReporter.h>
#include <data/DependencyInjector.h>

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

    struct IndexHandle
    {
        IndexType index : WORD_BIT - 1;
        IndexType is_valid : 1;
    };
    static_assert(sizeof(IndexHandle) == sizeof(IndexType), "IndexHandle must be 4 bytes");

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


    template<typename T>
    concept HasGetTag = requires(const T& t)
    {
        { t.get_tag() } -> std::same_as<TagType>;
    };

    // base class 
    template<typename SumStorageT>
    struct SumDispatcher
        : public InjectSingleton<ErrorReporter>
    {
    public:
        using Anchor = typename SumStorageT::Anchor;

    public:
        const SumStorageT& sum_storage;
        IndexHandle index;

    public:
        void validate() const
        {
            if (!index.is_valid)
            {
                USE_SAFE(ErrorReporter)->post_uncorrectable(
                    std::format("invalid sum reference: tag {}, index {}", tag, index.index));
            }
        }

    private:
        template<typename Self>
            requires HasGetTag<Self>
        const Self& self(this Self&& self) const
        {
            return self;
        }

    public:
        template<typename MethodT>
        decltype(auto) call() const
        {
            static constinit auto table =
                make_dispatch_table<MethodT, SumStorageT, typename SumStorageT::Type>();

            validate();
            return table[self().get_tag()](sum_storage, index);
        }

#define SUM_HANDLE_METHOD(name) \
        decltype(auto) name() const \
        { \
            return call<Method<&Anchor::name, decltype(&Anchor::name)>>(); \
        }

        SUM_HANDLE_METHOD(print)
        SUM_HANDLE_METHOD(evaluate)
        SUM_HANDLE_METHOD(optimize)
        SUM_HANDLE_METHOD(check_types)

#undef SUM_HANDLE_METHOD
    };


    // a type-specifc slim wrapper under the sum handle
    template<typename T, typename SumStorageT>
    struct SumReference 
        : public SumDispatcher<SumStorageT>
    {
    public:
        static constexpr TagType tag =
            TypeIndexV<T, typename SumStorageT::Type>;

        constexpr TagType get_tag() const 
        {
            return tag;
        }

    public:
        const T& get() const
        {
            return sum_storage.template get<T>()[index];
        }

        T& get()
        {
            return sum_storage.template get<T>()[index];
        }

        SumHandle<SumStorageT> erase() const
        {
            return SumHandle<SumStorageT>{ sum_storage, index, tag };
        }

    public:
        SumReference(const SumStorageT& sum_storage, IndexType index)
            : SumDispatcher<SumStorageT>{ sum_storage, { index, true } }
        {
            // valid reference
        }

        SumReference(const SumStorageT& sum_storage)
            : SumDispatcher<SumStorageT>{ sum_storage, { 0, false } }
        {
            // invalid reference
        }
    };

    // dynamic dispatch wrapper type for sum families
    template<typename SumStorageT>
    struct SumHandle
        : public SumDispatcher<SumStorageT>
    {
    public:
        TagType tag;

        constexpr TagType get_tag() const 
        {
            return tag;
        }

    private:
        template<typename T>
        void validate_tag() const
        {   
            // runtime error
            if (tag != TypeIndexV<T, typename SumStorageT::Type>)
            {
                USE_SAFE(ErrorReporter)->post_uncorrectable(
                    std::format("invalid sum reference: expected tag {}, actual tag {}, index {}",
                        TypeIndexV<T, typename SumStorageT::Type>, tag, index.index));
            }
        }

    public:
        template<typename T>
        const T& get() const
        {
            validate_tag<T>();
            return sum_storage.template get<T>()[index];
        }

        template<typename T>
        T& get()
        {
            validate_tag<T>();
            return sum_storage.template get<T>()[index];
        }

    public:
        SumHandle(const SumStorageT& sum_storage, IndexType index, TagType tag)
            : SumDispatcher<SumStorageT>{ sum_storage, { index, true } }, tag{ tag }
        {
            // valid handle
        }

        SumHandle(const SumStorageT& sum_storage)
            : SumDispatcher<SumStorageT>{ sum_storage, { 0, false } }, tag{ 0 }
        {
            // invalid handle
        }
    };

    // entry point into sum dynamic dispatch
    template<typename T, typename SumStorageT>
    SumReference<T, SumStorageT> make_reference(const SumStorageT& sum_storage, IndexType index)
    {
        return SumReference<T, SumStorageT>{ sum_storage, index };
    }

    template<typename T, typename SumStorageT>
    SumReference<T, SumStorageT> make_invalid_reference(const SumStorageT& sum_storage)
    {
        return SumReference<T, SumStorageT>{ sum_storage };
    }

    template<typename T, typename SumStorageT>
    SumHandle<SumStorageT> make_handle(const SumStorageT& sum_storage, IndexType index)
    {
        return SumHandle<SumStorageT>{ sum_storage, index, TypeIndexV<T, typename SumStorageT::Type> };
    }

    template<typename T, typename SumStorageT>
    SumHandle<SumStorageT> make_invalid_handle(const SumStorageT& sum_storage)
    {
        return SumHandle<SumStorageT>{ sum_storage };
    }
}

#endif
