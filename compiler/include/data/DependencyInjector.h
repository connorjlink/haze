#ifndef HAZE_DEPENDENCYINJECTOR_H
#define HAZE_DEPENDENCYINJECTOR_H

// Haze DependencyInjector.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using TypePointer = void*;

	struct TypeId
	{
	private:
		TypePointer id;

	private:
		// NOTE: must be private to enforce explicit creation through thread-safe means
		TypeId(TypePointer id)
			: id(id)
		{
		}

	public:
		auto operator<=>(const TypeId& t) const = default;

		std::size_t hash_code() const
		{
			return std::hash<TypePointer>{}(id);
		}

		template<typename T>
		static constexpr TypeId type_id_with_cvr()
		{
			// sizeof does not matter, this merely requires a unique address per type
			static std::uint8_t id;
			return { static_cast<TypePointer>(&id) };
		};
	};

	// NOTE: this cannot be constexpr because it relies on statis instance data!!
	// MSVC GIVES TERRIBLE ERRORS IF YOU TRY
	template<typename T>
	TypeId type_id()
	{
		using Type = std::remove_cvref_t<T>;
		return TypeId::type_id_with_cvr<Type>();
	};
}

// IMPORTANT NOTE: must be defined before the usage in the container below! hence the separate namespaces
template<>
struct std::hash<hz::TypeId>
{
	std::size_t operator()(const hz::TypeId& type_id) const
	{
		return type_id.hash_code();
	}
};

namespace hz
{
	// enforce static type checking for service/singleton tags to prevent invalid usage
	template<typename T>
	struct ServiceTag
	{
		// thread-scoped
	};

	template<typename T>
	struct ScopedTag
	{
		// request/injection-scoped
	};

	template<typename T>
	struct SingletonTag
	{
		// global-scoped
	};


	template<template<typename> typename TagT>
	struct ResourceContainer
	{
	private:
		std::unordered_map<TypeId, std::function<std::shared_ptr<void>()>> factories;
		// has to be a recursive mutex to allow the same thread to register nested services
		std::recursive_mutex mutex;

	public:
		template<typename T>
			requires std::derived_from<T, TagT<T>>
		void register_factory(std::function<std::shared_ptr<T>()> factory)
		{
			std::scoped_lock lock{ mutex };
			const auto id = type_id<T>();

			factories[id] = [factory]()
			{
				return std::static_pointer_cast<void>(factory());
			};
		}

		template<typename T>
			requires std::derived_from<T, TagT<T>>
		std::shared_ptr<T> create()
		{
			std::scoped_lock lock{ mutex };
			const auto id = type_id<T>();

			if (!factories.contains(id))
			{
				// no return
				// NOTE: cannot use error reporter since we don't know if it was constructed yet
				std::cerr << std::format("unregistered factory type with entity id `{}`", id.hash_code());
				std::exit(1);
			}

			return std::static_pointer_cast<T>(factories.at(id)());
		}

	public:
		static ResourceContainer& instance()
		{
			static ResourceContainer container;
			return container;
		}
	};

	struct ServiceContainer : public ResourceContainer<ServiceTag>
	{
	public:
		// polymorphic use only
		template<typename T, typename U>
			requires std::derived_from<T, ServiceTag<T>> and (std::derived_from<U, T> or std::is_same_v<T, U>)
		void register_factory(std::function<std::shared_ptr<U>()> factory)
		{
			std::scoped_lock lock{ mutex };
			const auto id = type_id<T>();

			factories[id] = [factory]()
			{
				return std::static_pointer_cast<void>(factory());
			};
		}
	};

	using ScopedContainer = ResourceContainer<ScopedTag>;


	struct SingletonContainer
	{
	private:
		std::unordered_map<TypeId, std::shared_ptr<void>> instances;
		// nested services are a lot less likely for singletons, but still posible maybe?
		std::recursive_mutex mutex;

	public:
		template<typename T, typename U = T>
			requires std::derived_from<T, SingletonTag<T>> and (std::derived_from<U, T> or std::is_same_v<T, U>)
		void register_instance(std::shared_ptr<U> instance)
		{
			std::scoped_lock lock{ mutex };
			instances[type_id<T>()] = std::static_pointer_cast<void>(instance);
		}

		template<typename T, typename... ArgumentsTs>
			requires std::derived_from<T, SingletonTag<T>>
		void register_singleton(ArgumentsTs&&... arguments)
		{
			register_instance<T>(std::make_shared<T>(std::forward<ArgumentsTs>(arguments)...));
		}

		// polymorphic only
		template<typename T, typename U, typename... ArgumentsTs>
			requires std::derived_from<T, SingletonTag<T>> and std::derived_from<U, T>
		void register_singleton_polymorphic(ArgumentsTs&&... arguments)
		{
			register_instance<T>(std::make_shared<U>(std::forward<ArgumentsTs>(arguments)...));
		}
		
#pragma message("TODO: move away from std::unordered_map singleton container to default/parameterized constructible")
		//template<typename T>
		//	requires std::derived_from<T, SingletonTag<T>>
		//std::shared_ptr<T> get()
		//{
		//	static std::shared_ptr<T> instance = std::make_shared<T>();
		//	return instance;
		//}


		// fetches an unsynchronized singleton handle. note
		// that accessing this handle is not thread-safe!
		template<typename T>
			requires std::derived_from<T, SingletonTag<T>>
		std::shared_ptr<T> get()
		{
			std::scoped_lock lock{ mutex };
			const auto id = type_id<T>();

			if (!instances.contains(id))
			{
				// no return
				std::cerr << std::format("unregistered global service type with entity id `{}`", id.hash_code());
				std::exit(1);
			}

			return std::static_pointer_cast<T>(instances.at(id));
		}

		// enforces a thread-safe synchronized call to a singleton service.
		// stalls all threads competing for the same resource, so use sparingly!
		template<typename T, typename U>
			requires std::derived_from<T, SingletonTag<T>>
		U&& run_synchronized(std::function<U&&(std::shared_ptr<T>)> callable)
		{
			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			auto pointer = instance().get<T>();
			return callable(pointer);
		}

		// this is very, very unsafe if multiple threads are expected to be able to access it
		// not just data races but data corruption!
		template<typename T, typename U>
			requires std::derived_from<T, SingletonTag<T>>
		U&& run_unsynchronized(std::function<U&&(std::shared_ptr<T>)> callable)
		{
			auto pointer = instance().get<T>();
			return callable(pointer);
		}

	public:
		static SingletonContainer& instance()
		{
			static SingletonContainer container;
			return container;
		}
	};

#define RUN_SYNC(x, y) SingletonContainer::instance().run_synchronized<x>(y)
#define RUN_UNSYNC(x, y) SingletonContainer::instance().run_unsynchronized<x>(y)


	// have to inject new instances per-thread starting at object graph root container to avoid races
	struct ThreadScope
	{
	private:
		// type erasure is fine since shared pointer stores type-specific deleter
		std::unordered_map<TypeId, std::shared_ptr<void>> instances;

	public:
		template<typename T>
			requires std::derived_from<T, ServiceTag<T>>
		std::shared_ptr<T> get()
		{
			const auto index = type_id<T>();

			if (instances.contains(index))
			{
				const auto& existing_instance = instances.at(index);
				return std::static_pointer_cast<T>(existing_instance);
			}

			const auto instance = ServiceContainer::instance().create<T>();
			instances[index] = instance;
			return instance;
		}
	};

	inline ThreadScope& using_thread()
	{
		// provide a unique service instance per thread
		static thread_local ThreadScope scope;
		return scope;
	}


	template<typename T>
	struct InjectServiceTag
	{
	};

	// mixin, e.g., MyClass : public InjectService<MyService> { } ... using_service<T>()->some_method()
	// NOTE: requires a complete type--include prerequisite headers before using!
	template<typename... Ts>
		requires (std::derived_from<Ts, ServiceTag<Ts>> and ...)
	struct InjectService : public InjectServiceTag<Ts>...
	{
	};

	template<typename T, typename CallerT>
		requires std::derived_from<CallerT, InjectServiceTag<T>> and std::derived_from<T, ServiceTag<T>>
	std::shared_ptr<T> using_service(const CallerT*)
	{
		return using_thread().get<T>();
	}
	// denoted unsafe because the call could fail if the service was not registered
	// NOTE: DO NOT DEFERENCE THE SERVICE POINTER BECAUSE IT MIGHT BE POLYMORPHIC!
#define REQUIRE_UNSAFE(x) using_thread().get<x>()
	// denoted safe because the caller struct has already injected the service successfully
	// NOTE: not explicitly thread-safe, so use with caution!
#define REQUIRE_SAFE(x) using_service<x>(this)


	template<typename T>
	struct InjectScopedTag
	{
	};

	// mixin, e.g., MyClass : public InjectScoped<MyService> { } ... using_scoped<T>()->some_method(), which creates a new resource instance
	// NOTE: requires a complete type--include prerequisite headers before using!
	template<typename... Ts>
		requires (std::derived_from<Ts, ScopedTag<Ts>> and ...)
	struct InjectScoped : public InjectScopedTag<Ts>...
	{
	};

	template<typename T, typename CallerT>
		requires std::derived_from<CallerT, InjectScopedTag<T>> and std::derived_from<T, ScopedTag<T>>
	std::shared_ptr<T> using_scoped(const CallerT*)
	{
		return ScopedContainer::instance().create<T>();
	}
	// denoted unsafe because the call could fail if the scoped resource was not registered
#define CREATE_UNSAFE(x) ScopedContainer::instance().create<x>()
	// denoted safe because the caller struct has already injected the scoped resource successfully
	// NOTE: not explicitly thread-safe, so use with caution!
#define CREATE_SAFE(x) using_scoped<x>(this)


	template<typename T>
	struct InjectSingletonTag
	{
	};

	// mixin, e.g., MyClass : public InjectSingleton<MyService> { } ... using_singleton<T>()->some_method()
	// NOTE: requires a complete type--include prerequisite headers before using!
	template<typename... Ts>
		requires (std::derived_from<Ts, SingletonTag<Ts>> and ...)
	struct InjectSingleton : public InjectSingletonTag<Ts>...
	{
	};

	// NOTE: CallerT required to choose ensure the current struct has a valid instance available to fetch
	template<typename T, typename CallerT>
		requires std::derived_from<CallerT, InjectSingletonTag<T>> and std::derived_from<T, SingletonTag<T>>
	std::shared_ptr<T> using_singleton(const CallerT*)
	{
		return SingletonContainer::instance().get<T>();
	}
	// denoted unsafe because the call could fail if the singleton was not registered
#define USE_UNSAFE(x) SingletonContainer::instance().get<x>()
	// denoted safe because the caller struct has already injected the singleton successfully
	// NOTE: not explicitly thread-safe, so use with caution!
#define USE_SAFE(x) using_singleton<x>(this)


#pragma message("TODO: Autoregistration is kinda dangerous so not sure if the following is ready for deployment yet")

	// template<typename T, bool AutoRegisterB, typename... ArgumentsTs>
	// struct ServiceTag
	// {
	// 	// this will be automatically called if there are no arguments to pass
	// 	ServiceTag(ArgumentsTs&&... arguments)
	// 	{
	// 		if constexpr (AutoRegisterB)
	// 		{
	// 			ServiceContainer::instance().register_factory<T>([arguments... = std::forward<ArgumentsTs>(arguments)]() mutable
	// 			{
	// 				return std::make_shared<T>(std::forward<ArgumentsTs>(arguments)...);
	// 			});
	// 		}
	// 	}
	// };

	// template<typename T, bool AutoRegisterB, typename... ArgumentsTs>
	// struct SingletonTag
	// {
	// 	// this will be automatically called if there are no arguments to pass
	// 	SingletonTag(ArgumentsTs&&... arguments)
	// 	{
	// 		if constexpr (AutoRegisterB)
	// 		{
	// 			SingletonContainer::instance().register_singleton<T>(
	// 				std::forward<ArgumentsTs>(arguments)...)
	// 		}
	// 	}
	// };
}

#endif
