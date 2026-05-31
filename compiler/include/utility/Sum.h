#ifndef HAZE_SUM_H
#define HAZE_SUM_H

#include <error/ErrorReporter.h>
#include <data/DependencyInjector.h>
#include <utility/Typing.h>

// Haze Sum.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// sum vector storage definitions: 255 subtypes, 4GiO nodes per subtype
	using TagType = std::uint8_t;
	using IndexType = std::uint32_t;

	struct IndexHandle
	{
		// 31 bits index, 1 bit flag ensured by static_assert
		IndexType index : (std::numeric_limits<unsigned char>::digits * sizeof(std::uint32_t)) - 1;
		IndexType is_valid : 1;
	};
	static_assert(sizeof(IndexHandle) == sizeof(IndexType), "IndexHandle must be 4 bytes");

	// dynamic dispatch generator for sum families
	template<typename Fn, typename SumStorageT, typename Tuple, std::size_t... Is>
	consteval auto make_dispatch_table_impl(std::index_sequence<Is...>)
	{
		return std::array
		{
			[](const SumStorageT& sum, IndexType index) -> typename Fn::ReturnType
			{
				using T = std::tuple_element_t<Is, Tuple>;
				const auto& vector = sum.storage.template get<T>();
				return (vector[index].*Fn::pointer)(sum);
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
		constexpr std::vector<T>& get()
		{
			constexpr auto I = TypeIndex<T, std::tuple<Ts...>>::value;
			return std::get<I>(storage);
		}

		template<typename T>
		constexpr const std::vector<T>& get() const
		{
			constexpr auto I = TypeIndex<T, std::tuple<Ts...>>::value;
			return std::get<I>(storage);
		}

	public:
		// NOTE: the return value for these functions is "wrong" per the STL, but it is useful for making external references to sum members
		template<typename T, typename... Args>
		constexpr IndexType emplace_back(Args&&... args)
		{
			const auto new_index = get<T>().size();
			get<T>().emplace_back(std::forward<Args>(args)...);
			return new_index;
		}

		template<typename T>
		constexpr IndexType push_back(T&& value)
		{
			const auto new_index = get<T>().size();
			get<T>().push_back(std::forward<T>(value));
			return new_index;
		}
	};


	// node dispatch type traits
	template<typename T, typename SumT, typename Fn>
	concept ImplementsMethod = requires(const T & node, const SumT & sum)
	{
		{ (node.*Fn::pointer)(sum) } -> std::same_as<typename Fn::ReturnType>;
	};

	template<typename T, typename SumT, typename MethodsT, std::size_t... Is>
	concept SumTupleImpl = (ImplementsMethod<T, SumT, std::tuple_element_t<Is, MethodsT>> && ...);

	template<typename T, typename SumT, typename MethodsT>
	concept SumTuple = SumTupleImpl<T, SumT, MethodsT, std::make_index_sequence<std::tuple_size_v<MethodsT>>{}>;


	// sum type and dispatch family
	template<typename T, typename SumT, template<typename> typename MethodsT, typename AnchorT>
	concept SumElement = SumTuple<T, SumT, MethodsT<AnchorT>>;

	template<std::size_t I, typename T>
	concept IsInRange = (I >= std::numeric_limits<TagType>::min() and I <= std::numeric_limits<TagType>::max());

	template<template<typename> typename MethodsT, typename... Ts>
		requires IsInRange<sizeof...(Ts), TagType>
	struct Sum
	{
	public:
		using Storage = SumStorage<Ts...>;
		using Type = typename Storage::Type;
		using Anchor = typename Storage::Anchor;

	public:
		template<std::size_t I>
		using TypeAt = std::tuple_element_t<I, Type>;

	public:
		using Methods = MethodsT<Anchor>;

	public:
		struct Storage : public SumStorage<Ts...>
		{
		} storage;
	};

	template<typename... Ts>
	struct SumTypeList
	{
		using FilteredTs = RemoveTypeT<void, Ts...>;
		using Type = AsTupleT<FilteredTs>;
		using Anchor = std::tuple_element_t<0, Type>;

		template<typename T>
		inline static constexpr auto Index = TypeIndexV<T, Type>;
	};

	template<template<typename> typename MethodsT, typename TypeListT>
	struct MakeSum;

	template<template<typename> typename MethodsT, typename TypeListT>
	struct MakeSumHelper;

	template<template<typename> typename MethodsT, typename... TypeListT>
	struct MakeSumHelper<MethodsT, TypeList<TypeListT...>>
	{
		using Type = Sum<MethodsT, TypeListT...>;
	};

	template<template<typename> typename MethodsT, typename... TypeListT>
	struct MakeSum<MethodsT, SumTypeList<TypeListT...>>
	{
		using Type = typename MakeSumHelper<MethodsT, typename SumTypeList<TypeListT...>::FilteredTs>::Type;
	};

	template<typename T>
	concept HasGetTag = requires(const T & t)
	{
		{ t.get_tag() } -> std::same_as<TagType>;
	};

	// base class for dispatching methods on sum members
	template<typename SumStorageT>
	struct SumDispatcher
		: public InjectSingleton<ErrorReporter>
	{
	public:
		const SumStorageT& sum_storage;
		IndexHandle index;

	public:
		constexpr bool is_valid() const
		{
			return index.is_valid;
		}

	public:
		constexpr void validate() const
		{
			if (!is_valid())
			{
				USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
					"invalid sum reference: tag {}, index {}", self().get_tag(), index.index));
			}
		}

	private:
		template<typename Self>
			requires HasGetTag<Self>
		constexpr const Self& self(this Self&& self)
		{
			return self;
		}

	public:
		template<typename MethodT>
		constexpr decltype(auto) call() const
		{
			static constinit auto table =
				make_dispatch_table<MethodT, SumStorageT, typename SumStorageT::Type>();

			validate();
			return table[self().get_tag()](sum_storage, index);
		}

		// derived SumDispatcherT classes need to provide every common sum method wrapper around call<MethodT>()
#define DEFINE_SUM_METHOD(name, returntype) \
		constexpr returntype name() const \
		{ \
			using Anchor = typename SumStorageT::Anchor; \
			return this->template call<Method<&Anchor::name, decltype(&Anchor::name)>>(); \
		}
	};

#define METHOD_TUPLE_ENTRY(name, returntype) Method<&AnchorT::name, decltype(&AnchorT::name)>,
#define SUM_DISPATCH_ENTRY(name, returntype) DEFINE_SUM_METHOD(name, returntype)

#define DEFINE_SUM_ANCILLARY(name, X) \
	template<typename SumStorageT> \
	class name##SumDispatcher : public SumDispatcher<SumStorageT> \
	{ \
	public: \
		X(SUM_DISPATCH_ENTRY) \
	}; \
	template<typename AnchorT> \
	using name##Methods = AllButLastT<std::tuple \
		< \
			X(METHOD_TUPLE_ENTRY) \
			void \
		> \
	>; \
	struct name##SumStorage; \
	using name##Handle = SumHandle<name##SumDispatcher, name##SumStorage>; \
	template<typename T> \
	using name##Reference = SumReference<T, name##SumDispatcher, name##SumStorage>; \
	using name##Facade = SumMemberBase<name##SumStorage>; 


	// dynamic dispatch wrapper type for sum families
	template<template<typename> typename SumDispatcherT, typename SumStorageT>
	struct SumHandle
		: public SumDispatcherT<SumStorageT>
	{
	public:
		TagType tag;

		constexpr TagType get_tag() const
		{
			return tag;
		}

	private:
		template<typename T>
		constexpr void validate_tag() const
		{
			static constexpr TagType expected_tag =
				TypeIndexV<T, typename SumStorageT::Type>;

			// runtime error
			if (tag != expected_tag)
			{
				this->USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
					"invalid sum reference: expected tag {}, actual tag {}, index {}",
						expected_tag, tag, this->index.index));
			}
		}

	public:
		template<typename T>
		constexpr const T& get() const
		{
			validate_tag<T>();
			return this->sum_storage.template get<T>()[this->index];
		}

		template<typename T>
		constexpr T& get()
		{
			validate_tag<T>();
			return this->sum_storage.template get<T>()[this->index];
		}

	public:
		constexpr SumHandle(const SumStorageT& sum_storage, IndexType index, TagType tag)
			: SumDispatcherT<SumStorageT>{ sum_storage, { index, true } }, tag{ tag }
		{
			// valid handle
		}

		constexpr SumHandle(const SumStorageT& sum_storage)
			: SumDispatcherT<SumStorageT>{ sum_storage, { 0, false } }, tag{ 0 }
		{
			// invalid handle
		}
	};

	// type-specifc slim wrapper for static dispatch on sum members
	template<typename T, template<typename> typename SumDispatcherT, typename SumStorageT>
	struct SumReference
		: public SumDispatcherT<SumStorageT>
	{
	public:
		constexpr TagType get_tag() const
		{
			return TypeIndexV<T, typename SumStorageT::Type>;
		}

	public:
		constexpr const T& get() const
		{
			return this->sum_storage.template get<T>()[this->index];
		}

		constexpr T& get()
		{
			return this->sum_storage.template get<T>()[this->index];
		}

		constexpr SumHandle<SumDispatcherT, SumStorageT> erase() const
		{
			return SumHandle<SumDispatcherT, SumStorageT>{ this->sum_storage, this->index, get_tag() };
		}

	public:
		constexpr SumReference(const SumStorageT& sum_storage, IndexType index)
			: SumDispatcherT<SumStorageT>{ sum_storage, { index, true } }
		{
			// valid reference
		}

		constexpr SumReference(const SumStorageT& sum_storage)
			: SumDispatcherT<SumStorageT>{ sum_storage, { 0, false } }
		{
			// invalid reference
		}
	};

	// entry point into sum dynamic dispatch
	template<typename T, template<typename> typename SumDispatcherT, typename SumStorageT>
	constexpr SumHandle<SumDispatcherT, SumStorageT> make_handle(const SumStorageT& sum_storage, T&& value)
	{
		// add it to the sum storage
		const auto index = sum_storage.template push_back<T>(std::forward<T>(value));
		return SumHandle<SumDispatcherT, SumStorageT>{ sum_storage, index, TypeIndexV<T, typename SumStorageT::Type> };
	}

	template<template<typename> typename SumDispatcherT, typename SumStorageT>
	constexpr SumHandle<SumDispatcherT, SumStorageT> make_invalid_handle(const SumStorageT& sum_storage)
	{
		// does not participate in sum storage
		return SumHandle<SumDispatcherT, SumStorageT>{ sum_storage };
	}

	template<typename T, template<typename> typename SumDispatcherT, typename SumStorageT>
	constexpr SumReference<T, SumDispatcherT, SumStorageT> make_reference(const SumStorageT& sum_storage, T&& value)
	{
		// add it to the sum storage
		const auto index = sum_storage.template push_back<T>(std::forward<T>(value));
		return SumReference<T, SumDispatcherT, SumStorageT>{ sum_storage, index };
	}

	template<typename T, template<typename> typename SumDispatcherT, typename SumStorageT>
	constexpr SumReference<T, SumDispatcherT, SumStorageT> make_invalid_reference(const SumStorageT& sum_storage)
	{
		// does not participate in sum storage
		return SumReference<T, SumDispatcherT, SumStorageT>{ sum_storage };
	}

	// common base class recommended for sum member classes
	template<typename SumFacadeT>
	struct SumMemberBase
	{
		// NOTE: this would be ideal to simplify usage but this cannot work due to circular dependency issues with the incomplete type facade
		// Instead, each implementation xSumBase class needs to define the common base
		//using Storage = typename SumFacadeT::Storage;

		template<typename Self>
		constexpr TagType tag_type(this Self&& self)
		{
			using This = std::remove_pointer_t<std::remove_cvref_t<Self>>;
			return TypeIndexV<This, typename SumFacadeT::Type>;
		}
	};
}

#endif
