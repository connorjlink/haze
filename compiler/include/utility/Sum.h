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
		TagType tag;

		constexpr IndexHandle(IndexType index, bool is_valid)
			: index{ index }, is_valid{ static_cast<IndexType>(is_valid) }
		{}
		constexpr IndexHandle(TagType tag, IndexType index, bool is_valid)
			: index{ index }, is_valid{ static_cast<IndexType>(is_valid) }, tag{ tag }
		{}
	};

	// dynamic dispatch generator for sum families
	template<typename MethodT, typename StorageT, typename TupleT, typename... ArgumentsTs, std::size_t... Is>
	consteval auto make_dispatch_table_implementation(std::index_sequence<Is...>, std::type_identity<void(ArgumentsTs...)>)
	{
		using ReturnType = typename MethodT::ReturnType;
		using FunctionPointerT = ReturnType(*)(const StorageT&, IndexType, ArgumentsTs...);

		return std::array<FunctionPointerT, sizeof...(Is)>
		{
			[](const StorageT& sum, IndexType index, ArgumentsTs... arguments) -> ReturnType
				{
					using T = std::tuple_element_t<Is, TupleT>;
					const auto& vector = sum.template get<T>();
					auto method_pointer = cast_method_pointer<T>(MethodT::pointer);
					return (vector[index].*method_pointer)(std::forward<ArgumentsTs>(arguments)...);
				}...
		};
	}

	template<typename MethodT, typename StorageT, typename TupleT>
	consteval auto make_dispatch_table()
	{
		using FunctionT = typename MethodT::FunctionT;

		return make_dispatch_table_implementation<MethodT, StorageT, TupleT>(
			std::make_index_sequence<std::tuple_size_v<TupleT>>{},
			std::type_identity<FunctionT>{});
	}


	// type-checked discriminated linear sum storage for statically known sum families
	template<typename... Ts>
	struct Storage
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
		template<typename T, typename... ArgumentsTs>
		constexpr IndexType emplace_back(ArgumentsTs&&... arguments)
		{
			const auto new_index = get<T>().size();
			get<T>().emplace_back(std::forward<ArgumentsTs>(arguments)...);
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
	template<typename Node, typename FunctionT, typename Signature>
	struct ImplementsMethodSignature : std::false_type {};

	template<typename Node, typename FunctionT, typename... ArgumentsTs>
	struct ImplementsMethodSignature<Node, FunctionT, void(ArgumentsTs...)>
	{
		static constexpr bool value = requires(const Node & node, ArgumentsTs... arguments)
		{
			{ (node.*cast_method_pointer<Node>(FunctionT::pointer))(std::forward<ArgumentsTs>(arguments)...) } -> std::same_as<typename FunctionT::ReturnType>;
		};
	};


	template<typename T, typename SumT, typename MethodT>
	concept ImplementsMethod = ImplementsMethodSignature<T, MethodT, typename MethodT::FunctionT>::value;

	template<typename T, typename SumT, typename MethodsT, std::size_t... Is>
	concept SumTupleImplementation = (ImplementsMethod<T, SumT, std::tuple_element_t<Is, MethodsT>> and ...);

	template<typename T, typename SumT, typename MethodsT>
	concept SumTuple = SumTupleImplementation < T, SumT, MethodsT, std::make_index_sequence<std::tuple_size_v<MethodsT>>{} > ;


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
		using Storage = Storage<Ts...>;
		using Type = typename Storage::Type;
		using Anchor = typename Storage::Anchor;

	public:
		template<std::size_t I>
		using TypeAt = std::tuple_element_t<I, Type>;

	public:
		using Methods = MethodsT<Anchor>;

	public:
		struct Storage : public Storage<Ts...>
		{} storage;
	};

	template<typename... Ts>
	struct SumTypeList
	{
		using FilteredTs = RemoveTypeT<void, Ts...>;
		using Type = AsTupleT<FilteredTs>;
		using Anchor = std::tuple_element_t<0, Type>;

		template<typename T>
		inline constexpr auto Index = TypeIndexV<T, Type>;
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

	// safety checks to verify that the bitfields and tag pack properly to minimize waste since References and Handles will be passed around by copy
	static_assert(sizeof(IndexHandle) <= alignof(void*), "IndexHandle must be no larger than the platform reference alignment to avoid padding");

	// base struct for dispatching methods on sum members, no handle to 
	template<typename StorageT>
	struct SumDispatcherLite
		: public InjectSingleton<ErrorReporter>
	{
	public:
		IndexHandle index;

	public:
		constexpr bool is_valid() const
		{
			return index.is_valid;
		}
		constexpr operator bool() const
		{
			return is_valid();
		}

	public:
		template<typename Self>
		constexpr void validate(this Self&& self)
		{
			if (!self.is_valid())
			{
				// cannot use USE_SAFE macro due to this being unavailable in explicit object parameter function
				using_singleton<ErrorReporter>(&self)->post_uncorrectable(std::format(
					"invalid sum reference: tag {}, index {}", self.get_tag(), static_cast<IndexType>(self.index.index)), NULL_TOKEN);
			}
		}

		// NOTE: the following template order is mandatory to deduce template arguments without errors due to compiler bug with 
		template<typename T, typename StorageT, typename Self>
		constexpr decltype(auto) get(this Self&& self, StorageT&& storage)
		{
			self.template validate_tag<T>();
			return std::forward<StorageT>(storage).template get<T>()[self.index.index];
		}

	public:
		// NOTE: the following template order is mandatory to deduce template arguments without errors due to compiler bug with MSVC145

		template<typename MethodT, typename Self, typename... ArgumentsTs>
		constexpr decltype(auto) call(this Self&& self, ArgumentsTs&&... arguments)
		{
			static constinit auto table =
				make_dispatch_table<MethodT, StorageT, typename StorageT::Type>();

			self.validate();
			return table[self.get_tag()](self.sum_storage, self.index.index, std::forward<ArgumentsTs>(arguments)...);
		}

	public:
		constexpr SumDispatcherLite(IndexHandle handle, TagType tag)
			: index{ tag, handle.index, static_cast<bool>(handle.is_valid) }
		{
		}
	};

	// required not to have std::initializer_list constructor for std::vector interfere with references/handles when forwarding
	template<typename T, template<typename> typename SumDispatcherT, typename StorageT, typename... ArgumentsTs>
	concept IsValidDispatchable = (
		(not std::same_as<std::remove_cvref_t<ArgumentsTs>, T> and ...)
		and std::constructible_from<SumDispatcherT<StorageT>, IndexHandle, TagType, ArgumentsTs...>);;

	template<typename StorageT>
	struct SumDispatcher : public SumDispatcherLite<StorageT>
	{
	public:
		StorageT& sum_storage;

	public:
		template<typename T, typename Self>
		constexpr decltype(auto) get(this Self&& self)
		{
			return std::forward<Self>(self).template SumDispatcherLite<StorageT>::template get<T>(
				std::forward_like<Self>(self.sum_storage));
		}

	public:
		constexpr SumDispatcher(IndexHandle handle, TagType tag, const StorageT& sum_storage)
			: SumDispatcherLite<StorageT>{ handle, tag }, sum_storage{ sum_storage }
		{
		}
	};


	// required to constrain handle/reference functions later without a mandatory template parameter
	struct SumMemberBaseTag
	{
	};

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


	template<typename T>
	concept IsFormattableSum = requires(const T & t)
	{
		{ t.format() } -> std::convertible_to<std::string>;
		std::derived_from<T, SumMemberBaseTag>;
	};

	template<typename T>
	concept IsFormattableDispatcher = requires(const T & t)
	{
		{ t.get_tag() } -> std::convertible_to<TagType>;
		{ t.format() } -> std::convertible_to<std::string>;
		std::derived_from<T, SumDispatcher<typename T::Storage>>;
	};
}

template<typename T>
	requires hz::IsFormattableSum<T> or hz::IsFormattableDispatcher<T>
struct std::formatter<T>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}

	auto format(const T& value, std::format_context& context) const
	{
		return std::format_to(context.out(), "{}", value.format());
	}
};

namespace hz
{
#define METHOD_TUPLE_ENTRY(name, returntype) Method<&AnchorT::name>,
#define SUM_DISPATCH_ENTRY(name, returntype) \
	template<typename Self, typename... ArgumentsTs> \
	constexpr decltype(auto) name(this Self&& self, ArgumentsTs&&... arguments) \
	{ \
		using Anchor = typename StorageT::Anchor; \
		return self.template call<Method<&Anchor::name>>(std::forward<ArgumentsTs>(arguments)...); \
	}

#define DEFINE_SUM(name, methods) \
	template<typename StorageT> \
	struct name##SumDispatcher : public SumDispatcher<StorageT> \
	{ \
	public: \
		methods(SUM_DISPATCH_ENTRY, name##Handle) \
	public: \
		constexpr name##SumDispatcher(IndexHandle index, TagType tag, const StorageT& sum_storage) \
			: SumDispatcher<StorageT>{ index, tag, sum_storage } \
		{ \
		} \
	}; \
	template<typename AnchorT> \
	using name##Methods = AllButLastT \
	< \
		methods(METHOD_TUPLE_ENTRY, name##Handle) \
		void \
	>; \
	struct name##Storage; \
	using name##Handle = SumHandle<name##SumDispatcher, name##Storage>; \
	template<typename T> \
	using name##Reference = SumReference<T, name##SumDispatcher, name##Storage>; \
	using name##HandleLite = SumHandle<SumDispatcherLite, name##Storage>; \
	template<typename T> \
	using name##ReferenceLite = SumReference<T, SumDispatcherLite, name##Storage>; \
	using name##Facade = SumMemberBase<name##Storage>; 

#define FORWARD_DECLARE_SUM(name) \
	template<template<typename> typename SumDispatcherT, typename StorageT> \
	struct SumHandle; \
	template<typename T, template<typename> typename SumDispatcherT, typename StorageT> \
	struct SumReference; \
	template<typename StorageT> \
	struct name##SumDispatcher; \
	struct name##Storage; \
	using name##Handle = SumHandle<name##SumDispatcher, name##Storage>; \
	template<typename T> \
	using name##Reference = SumReference<T, name##SumDispatcher, name##Storage>; \
	using name##HandleLite = SumHandle<SumDispatcherLite, name##Storage>; \
	template<typename T> \
	using name##ReferenceLite = SumReference<T, SumDispatcherLite, name##Storage>; \
	using name##Facade = SumMemberBase<name##Storage>;


	// dynamic dispatch wrapper type for sum families
	template<template<typename> typename SumDispatcherT, typename StorageT>
	struct SumHandle : public SumDispatcherT<StorageT>
	{
	public:
		constexpr TagType get_tag() const
		{
			return this->index.tag;
		}

	public:
		template<typename T>
		constexpr void validate_tag() const
		{
			static constexpr TagType expected_tag =
				TypeIndexV<T, typename StorageT::Type>;

			// runtime error
			if (this->index.tag != expected_tag)
			{
				USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
					"invalid sum reference: expected tag {}, actual tag {}, index {}",
						expected_tag, this->index.tag, static_cast<IndexType>(this->index.index)), NULL_TOKEN);
			}
		}

	public:
		template<typename... ArgumentsTs>
			requires IsValidDispatchable<SumHandle, SumDispatcherT, StorageT, ArgumentsTs...>
		constexpr SumHandle(IndexType index, TagType tag, ArgumentsTs&&... arguments)
			: SumDispatcherT<StorageT>{ IndexHandle{ index, true }, tag, std::forward<ArgumentsTs>(arguments)... }
		{
			// valid handle
		}

		template<typename... ArgumentsTs>
			requires IsValidDispatchable<SumHandle, SumDispatcherT, StorageT, ArgumentsTs...>
		constexpr SumHandle(ArgumentsTs&&... arguments)
			: SumDispatcherT<StorageT>{ IndexHandle{ 0, false }, TagType{ 0 }, std::forward<ArgumentsTs>(arguments)... }
		{
			// invalid handle
		}
	};

	// type-specifc slim wrapper for static dispatch on sum members
	template<typename T, template<typename> typename SumDispatcherT, typename StorageT>
	struct SumReference : public SumDispatcherT<StorageT>
	{
	public:
		constexpr TagType get_tag() const
		{
			return TypeIndexV<T, typename StorageT::Type>;
		}

	public:
		constexpr SumHandle<SumDispatcherT, StorageT> erase() const
		{
			return SumHandle<SumDispatcherT, StorageT>{ this->sum_storage, this->index, get_tag() };
		}

	public:
		template<typename... ArgumentsTs>
			requires IsValidDispatchable<SumReference, SumDispatcherT, StorageT, ArgumentsTs...>
		constexpr SumReference(IndexType index, ArgumentsTs&&... arguments)
			: SumDispatcherT<StorageT>{ IndexHandle{ index, true }, get_tag(), std::forward<ArgumentsTs>(arguments)... }
		{
			// valid reference
		}

		template<typename... ArgumentsTs>
			requires IsValidDispatchable<SumReference, SumDispatcherT, StorageT, ArgumentsTs...>
		constexpr SumReference(ArgumentsTs&&... arguments)
			: SumDispatcherT<StorageT>{ IndexHandle{ 0, false }, get_tag(), std::forward<ArgumentsTs>(arguments)... }
		{
			// invalid reference
		}
	};


	//////////////////////////////////////////////////////
	// Entry point into sum dynamic dispatch
	//////////////////////////////////////////////////////

	// NOTE: typically the value macro parameter should be std::move()'d to avoid creating an additional copy of what should be temporary

	template<typename T, template<typename> typename SumDispatcherT, typename StorageT>
	constexpr SumHandle<SumDispatcherT, StorageT> make_handle(StorageT& sum_storage, T&& value)
	{
		// add it to the sum storage
		const auto index = sum_storage.template push_back<T>(std::forward<T>(value));
		return SumHandle<SumDispatcherT, StorageT>{ sum_storage, index, TypeIndexV<T, typename StorageT::Type> };
	}
#define MAKE_HANDLE(type, family, sum, value) make_handle<type, family##SumDispatcher, family##Storage>(sum, value)

	template<typename T>
		requires std::derived_from<T, SumMemberBaseTag>
	constexpr SumHandle<typename T::Dispatcher, typename T::Storage> make_handle(typename T::Storage& sum_storage, T&& value)
	{
		return make_handle<T, typename T::Dispatcher, typename T::Storage>(sum_storage, std::forward<T>(value));
	}
#define MAKE_HANDLE_AUTO(sum, value) make_handle(sum, value)

	template<template<typename> typename SumDispatcherT, typename StorageT>
	constexpr SumHandle<SumDispatcherT, StorageT> make_invalid_handle(const StorageT& sum_storage)
	{
		// does not participate in sum storage
		static const auto invalid_handle = SumHandle<SumDispatcherT, StorageT>{ sum_storage };
		return invalid_handle;
	}
#define MAKE_INVALID_HANDLE(sum, family) make_invalid_handle<family##SumDispatcher, family##Storage>(sum)

	template<typename T>
		requires std::derived_from<T, SumMemberBaseTag>
	constexpr SumHandle<typename T::Dispatcher, typename T::Storage> make_invalid_handle(const typename T::Storage& sum_storage)
	{
		return make_invalid_handle<typename T::Dispatcher, typename T::Storage>(sum_storage);
	}
#define MAKE_INVALID_HANDLE_AUTO(sum, type) make_invalid_handle<type>(sum)


	template<typename T, template<typename> typename SumDispatcherT, typename StorageT>
	constexpr SumReference<T, SumDispatcherT, StorageT> make_reference(StorageT& sum_storage, T&& value)
	{
		// add it to the sum storage
		const auto index = sum_storage.template push_back<T>(std::forward<T>(value));
		return SumReference<T, SumDispatcherT, StorageT>{ sum_storage, index };
	}
#define MAKE_REFERENCE(type, family, sum, value) make_reference<type, family##SumDispatcher, family##Storage>(sum, value)

	template<typename T>
		requires std::derived_from<T, SumMemberBaseTag>
	constexpr SumReference<T, typename T::Dispatcher, typename T::Storage> make_reference(typename T::Storage& sum_storage, T&& value)
	{
		return make_reference<T, typename T::Dispatcher, typename T::Storage>(sum_storage, std::forward<T>(value));
	}
#define MAKE_REFERENCE_AUTO(sum, value) make_reference(sum, value)

	template<typename T, template<typename> typename SumDispatcherT, typename StorageT>
	constexpr SumReference<T, SumDispatcherT, StorageT> make_invalid_reference(const StorageT& sum_storage)
	{
		// does not participate in sum storage
		static const auto invalid_reference = SumReference<T, SumDispatcherT, StorageT>{ sum_storage };
		return invalid_reference;
	}
#define MAKE_INVALID_REFERENCE(type, family, sum) make_invalid_reference<type, family##SumDispatcher, family##Storage>(sum)

	template<typename T>
		requires std::derived_from<T, SumMemberBaseTag>
	constexpr SumReference<T, typename T::Dispatcher, typename T::Storage> make_invalid_reference(const typename T::Storage& sum_storage)
	{
		return make_invalid_reference<T, typename T::Dispatcher, typename T::Storage>(sum_storage);
	}
#define MAKE_INVALID_REFERENCE_AUTO(sum, type) make_invalid_reference<type>(sum)
}

#endif
