#ifndef HAZE_TYPING_H
#define HAZE_TYPING_H

// Haze Typing.h
// (c) Connor J. Link. All Rights Reserved. 

namespace hz
{
	// meta type traits
	template<typename T, typename Types>
	struct TypeIndex;

	template<typename T, typename... Types>
	struct TypeIndex<T, std::tuple<Types...>>
	{
		static constexpr std::size_t value = []<std::size_t... Is>(std::index_sequence<Is...>)
		{
			auto index = sizeof...(Types);
			// fold on setting the index to the matching type or nothing if not found
			((std::is_same_v<T, std::tuple_element_t<Is, std::tuple<Types...>>> ? index = Is : 0), ...);
			return index;
		}(std::make_index_sequence<sizeof...(Types)>{});

		static_assert(value < sizeof...(Types), "The specified type was not found within the provided tuple");
	};

	template<typename T, typename TupleT>
	inline constexpr auto TypeIndexV = TypeIndex<T, TupleT>::value;


	// function type traits
	template<auto MethodPointer, typename Signature>
	struct Method;

	template<auto MethodPointer, typename R, typename... Args>
	struct Method<MethodPointer, R(Args...)>
	{
		static constexpr auto pointer = MethodPointer;
		using ReturnType = R;
	};


	template<typename T>
	struct MethodTraits;

	template<typename R, typename C, typename... Args>
	struct MethodTraits<R(C::*)(Args...)>
	{
		using FunctionT = void(Args...);
	};

	template<typename R, typename C, typename... Args>
	struct MethodTraits<R(C::*)(Args...) const>
	{
		using FunctionT = void(Args...);
	};

	template<typename R, typename C, typename... Args>
	struct MethodTraits<R(C::*)(Args...) noexcept>
	{
		using FunctionT = void(Args...);
	};

	template<typename R, typename C, typename... Args>
	struct MethodTraits<R(C::*)(Args...) const noexcept>
	{
		using FunctionT = void(Args...);
	};


	template<typename... Ts>
	struct TypeList
	{
	};


	template<typename ToRemoveT, typename... Ts>
	struct RemoveType;

	template<typename ToRemoveT>
	struct RemoveType<ToRemoveT>
	{
		// base case, the input list is exhausted
		using Type = TypeList<>;
	};

	template<typename ToRemoveT, typename T, typename... Ts>
	struct RemoveType<ToRemoveT, T, Ts...>
	{
		using Rest = typename RemoveType<ToRemoveT, Ts...>::Type;

		// base case: at the head for prepending
		template<typename U, typename Us>
		struct Prepend;

		template<typename U, typename... Us>
		struct Prepend<U, TypeList<Us...>>
		{
			using Type = TypeList<U, Us...>;
		};

		using Type = std::conditional_t
		<
			std::is_same_v<ToRemoveT, T>,
			Rest,
			typename Prepend<T, Rest>::Type
		>;
	};

	template<typename ToRemoveT, typename... Ts>
	using RemoveTypeT = typename RemoveType<ToRemoveT, Ts...>::Type;


	template<typename T>
	struct AsTuple;

	template<typename... Ts>
	struct AsTuple<TypeList<Ts...>>
	{
		using Type = std::tuple<Ts...>;
	};

	template<typename T>
	using AsTupleT = typename AsTuple<T>::Type;


	template<typename>
	struct AllButLast;

	template<typename... Ts>
	struct AllButLast<std::tuple<Ts...>>
	{
	private:
		template<std::size_t... Is>
		static auto helper(std::index_sequence<Is...>)
			-> std::tuple<std::tuple_element_t<Is, std::tuple<Ts...>>...>;

	public:
		using type = std::conditional_t
		<
			(sizeof...(Ts) > 0),
			decltype(helper(std::make_index_sequence<sizeof...(Ts) - 1>{})),
			std::tuple<>
		>;
	};

	template<typename... Ts>
	using AllButLastT = typename AllButLast<std::tuple<Ts...>>::type;


	template<template<typename...> typename Tuple1, template<typename...> typename Tuple2>
	using TupleCatT = decltype(std::tuple_cat(
		std::declval<Tuple1>(),
		std::declval<Tuple2>()));
}

#endif
