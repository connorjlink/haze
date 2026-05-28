#ifndef HAZE_TYPING_H
#define HAZE_TYPING_H

// Haze Typing.h
// (c) Connor J. Link. All Rights Reserved. 

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
	inline constexpr auto TypeIndexV = TypeIndex<T, Tuple>::value;


	// function type traits
	template<auto MethodPointer, typename Signature>
	struct Method;

	template<auto MethodPointer, typename R, typename... Args>
	struct Method<MethodPointer, R(Args...)>
	{
		static constexpr auto pointer = MethodPointer;
		using ReturnType = R;
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
}

#endif
