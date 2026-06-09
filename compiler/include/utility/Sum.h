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

		constexpr IndexHandle(IndexType index, bool is_valid)
			: index{ index }, is_valid{ static_cast<IndexType>(is_valid) }
		{
		}
	};
	static_assert(sizeof(IndexHandle) == sizeof(IndexType), "IndexHandle must be 4 bytes");

	// dynamic dispatch generator for sum families
	template<typename MethodT, typename SumStorageT, typename TupleT, std::size_t... Is>
	consteval auto make_dispatch_table_impl(std::index_sequence<Is...>)
	{
		return std::array
		{
			[](const SumStorageT& sum, IndexType index) -> typename MethodT::ReturnType
			{
				using T = std::tuple_element_t<Is, TupleT>;
				const auto& vector = sum.storage.template get<T>();
				return (vector[index].*MethodT::pointer)(sum);
			}...
		};
	}

	template<typename MethodT, typename SumStorageT, typename TupleT>
	consteval auto make_dispatch_table()
	{
		return make_dispatch_table_impl<MethodT, SumStorageT, TupleT>(
			std::make_index_sequence<std::tuple_size_v<TupleT>>{});
	}


	// type-checked discriminated linear sum storage for statically known sum families
	template<typename... Ts>
	struct SumStorage
	{
	public:
		static constexpr std::size_t size = sizeof...(Ts);

	public:
		using Type = std::tuple<Ts...>;
		using Anchor = std::tuple_element_t<0, Type>;

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
			return static_cast<IndexType>(new_index);
		}
	};


	// node dispatch type traits
	template<typename T, typename SumT, typename Fn>
	concept ImplementsMethod = requires(const T & node, const SumT & sum)
	{
		{ (node.*Fn::pointer)(sum) } -> std::same_as<typename Fn::ReturnType>;
	};

	template<typename T, typename SumT, typename MethodsT, std::size_t... Is>
	concept SumTupleImplementation = (ImplementsMethod<T, SumT, std::tuple_element_t<Is, MethodsT>> && ...);

	template<typename T, typename SumT, typename MethodsT>
	concept SumTuple = SumTupleImplementation<T, SumT, MethodsT, std::make_index_sequence<std::tuple_size_v<MethodsT>>{}>;


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

	// base struct for dispatching methods on sum members
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
		template<typename Self>
		constexpr void validate(this Self&& self)
		{
			if (!self.is_valid())
			{
				USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
					"invalid sum reference: tag {}, index {}", self.get_tag(), index.index));
			}
		}

	public:
		template<typename MethodT, typename Self>
		constexpr decltype(auto) call(this Self&& self)
		{
			static constinit auto table =
				make_dispatch_table<MethodT, SumStorageT, typename SumStorageT::Type>();

			self.validate();
			return table[self.get_tag()](self.sum_storage, self.index);
		}

	public:
		constexpr SumDispatcher(const SumStorageT& sum_storage, IndexHandle index)
			: sum_storage{ sum_storage }, index{ index }
		{
		}
	};

	// required to constrain handle/reference functions later without a mandatory template parameter
	struct SumMemberBaseTag {};

	// common base struct recommended for sum member classes
	template<typename SumFacadeT>
	struct SumMemberBase : public SumMemberBaseTag
	{
		// NOTE: this would be ideal to simplify usage but this cannot work due to circular dependency issues with the incomplete type facade
		// Instead, each implementation xSumBase struct needs to define the common base
		//using Storage = typename SumFacadeT::Storage;

		template<typename Self>
		constexpr TagType tag_type(this Self&& self)
		{
			using This = std::remove_pointer_t<std::remove_cvref_t<Self>>;
			return TypeIndexV<This, typename SumFacadeT::Type>;
		}
	};


#define METHOD_TUPLE_ENTRY(name, returntype) Method<&AnchorT::name, decltype(&AnchorT::name)>,
#define SUM_DISPATCH_ENTRY(name, returntype) \
		template<typename Self> \
		constexpr decltype(auto) name(this Self&& self) \
		{ \
			using Anchor = typename SumStorageT::Anchor; \
			return self.template call<Method<&Anchor::name, decltype(&Anchor::name)>>(); \
		}

#define DEFINE_SUM(name, methods) \
	template<typename SumStorageT> \
	struct name##SumDispatcher : public SumDispatcher<SumStorageT> \
	{ \
	public: \
		methods(SUM_DISPATCH_ENTRY, name##Handle) \
	public: \
		constexpr name##SumDispatcher(const SumStorageT& sum_storage, IndexHandle index) \
			: SumDispatcher<SumStorageT>{ sum_storage, index } \
		{ \
		} \
	}; \
	template<typename AnchorT> \
	using name##Methods = AllButLastT \
	< \
		methods(METHOD_TUPLE_ENTRY, name##Handle) \
		void \
	>; \
	struct name##SumStorage; \
	using name##Handle = SumHandle<name##SumDispatcher, name##SumStorage>; \
	template<typename T> \
	using name##Reference = SumReference<T, name##SumDispatcher, name##SumStorage>; \
	using name##Facade = SumMemberBase<name##SumStorage>; 

#define FORWARD_DECLARE_SUM(name) \
	template<template<typename> typename SumDispatcherT, typename SumStorageT> \
	struct SumHandle; \
	template<typename SumStorageT> \
	struct name##SumDispatcher; \
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
			: SumDispatcherT<SumStorageT>{ sum_storage, IndexHandle{ index, true } }, tag{ tag }
		{
			// valid handle
		}

		constexpr SumHandle(const SumStorageT& sum_storage)
			: SumDispatcherT<SumStorageT>{ sum_storage, IndexHandle{ 0, false } }, tag{ 0 }
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
			: SumDispatcherT<SumStorageT>{ sum_storage, IndexHandle{ index, true } }
		{
			// valid reference
		}

		constexpr SumReference(const SumStorageT& sum_storage)
			: SumDispatcherT<SumStorageT>{ sum_storage, IndexHandle{ 0, false } }
		{
			// invalid reference
		}
	};


	//////////////////////////////////////////////////////
	// Entry point into sum dynamic dispatch
	//////////////////////////////////////////////////////

	// NOTE: typically the value macro parameter should be std::move()'d to avoid creating an additional copy of what should be temporary

	template<typename T, template<typename> typename SumDispatcherT, typename SumStorageT>
	constexpr SumHandle<SumDispatcherT, SumStorageT> make_handle(SumStorageT& sum_storage, T&& value)
	{
		// add it to the sum storage
		const auto index = sum_storage.template push_back<T>(std::forward<T>(value));
		return SumHandle<SumDispatcherT, SumStorageT>{ sum_storage, index, TypeIndexV<T, typename SumStorageT::Type> };
	}
#define MAKE_HANDLE(type, family, sum, value) make_handle<type, family##SumDispatcher, family##SumStorage>(sum, value)

	template<typename T>
		requires std::derived_from<T, SumMemberBaseTag>
	constexpr SumHandle<typename T::Dispatcher, typename T::Storage> make_handle(typename T::Storage& sum_storage, T&& value)
	{
		return make_handle<T, typename T::Dispatcher, typename T::Storage>(sum_storage, std::forward<T>(value));
	}
#define MAKE_HANDLE_AUTO(sum, value) make_handle(sum, value)

	template<template<typename> typename SumDispatcherT, typename SumStorageT>
	constexpr SumHandle<SumDispatcherT, SumStorageT> make_invalid_handle(const SumStorageT& sum_storage)
	{
		// does not participate in sum storage
		return SumHandle<SumDispatcherT, SumStorageT>{ sum_storage };
	}
#define MAKE_INVALID_HANDLE(sum, family) make_invalid_handle<family##SumDispatcher, family##SumStorage>(sum)

	template<typename T>
		requires std::derived_from<T, SumMemberBaseTag>
	constexpr SumHandle<typename T::Dispatcher, typename T::Storage> make_invalid_handle(const typename T::Storage& sum_storage)
	{
		return make_invalid_handle<typename T::Dispatcher, typename T::Storage>(sum_storage);
	}
#define MAKE_INVALID_HANDLE_AUTO(sum, type) make_invalid_handle<type>(sum)


	template<typename T, template<typename> typename SumDispatcherT, typename SumStorageT>
	constexpr SumReference<T, SumDispatcherT, SumStorageT> make_reference(SumStorageT& sum_storage, T&& value)
	{
		// add it to the sum storage
		const auto index = sum_storage.template push_back<T>(std::forward<T>(value));
		return SumReference<T, SumDispatcherT, SumStorageT>{ sum_storage, index };
	}
#define MAKE_REFERENCE(type, family, sum, value) make_reference<type, family##SumDispatcher, family##SumStorage>(sum, value)

	template<typename T>
		requires std::derived_from<T, SumMemberBaseTag>
	constexpr SumReference<T, typename T::Dispatcher, typename T::Storage> make_reference(typename T::Storage& sum_storage, T&& value)
	{
		return make_reference<T, typename T::Dispatcher, typename T::Storage>(sum_storage, std::forward<T>(value));
	}
#define MAKE_REFERENCE_AUTO(sum, value) make_reference(sum, value)

	template<typename T, template<typename> typename SumDispatcherT, typename SumStorageT>
	constexpr SumReference<T, SumDispatcherT, SumStorageT> make_invalid_reference(const SumStorageT& sum_storage)
	{
		// does not participate in sum storage
		return SumReference<T, SumDispatcherT, SumStorageT>{ sum_storage };
	}
#define MAKE_INVALID_REFERENCE(type, family, sum) make_invalid_reference<type, family##SumDispatcher, family##SumStorage>(sum)

	template<typename T>
		requires std::derived_from<T, SumMemberBaseTag>
	constexpr SumReference<T, typename T::Dispatcher, typename T::Storage> make_invalid_reference(const typename T::Storage& sum_storage)
	{
		return make_invalid_reference<T, typename T::Dispatcher, typename T::Storage>(sum_storage);
	}
#define MAKE_INVALID_REFERENCE_AUTO(sum, type) make_invalid_reference<type>(sum)
}

#endif
