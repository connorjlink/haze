#ifndef HAZE_DEPENDENCYINJECTOR_H
#define HAZE_DEPENDENCYINJECTOR_H

// Haze DependencyInjector.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using TypeIndexId = void*;

	class TypeIndex
	{
	private:
		TypeIndexId id;

	private:
		// NOTE: must be private to enforce explicit creation through thread-safe means
		TypeIndex(TypeIndexId id)
			: id(id)
		{
		}

	public:
		auto operator<=>(const TypeIndex& t) const = default;

		std::size_t hash_code() const
		{
			return std::hash<TypeIndexId>{}(id);
		}

		template<typename T>
		static constexpr TypeIndex type_id_with_cvr()
		{
			static int id;
			return { static_cast<TypeIndexId>(&id) };
		};
	};

	// NOTE: this cannot be constexpr because it relies on statis instance data!!
	// MSVC GIVES TERRIBLE ERRORS IF YOU TRY
	template<typename T>
	TypeIndex type_id()
	{
		using Type = std::remove_cvref_t<T>;
		return TypeIndex::type_id_with_cvr<Type>();
	};
}

namespace std
{
	// IMPORTANT NOTE: must be defined before the usage in the container below! hence the separate namespace
	// std extension provides a clean means by which to interop with containers (like maps)
	template<>
	struct hash<hz::TypeIndex>
	{
		size_t operator()(const hz::TypeIndex& t) const
		{
			return t.hash_code();
		}
	};
}

namespace hz
{
	// enforce static type checking for service/singleton tags to prevent invalid usage
	template<typename T>
	struct ServiceTag {};

	template<typename T>
	struct SingletonTag {};

	class ServiceContainer
	{
	private:
		std::unordered_map<TypeIndex, std::function<std::shared_ptr<void>()>> factories;
		// has to be a recursive mutex to allow the same thread to register nested services
		std::recursive_mutex mutex;

	public:
		template<typename T>
			requires std::derived_from<T, ServiceTag<T>>
		void register_factory(std::function<std::shared_ptr<T>()> factory)
		{
			std::scoped_lock lock{ mutex };
			const auto id = type_id<T>();

			factories[id] = [factory]()
			{
				return std::static_pointer_cast<void>(factory());
			};
		}

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

		template<typename T>
			requires std::derived_from<T, ServiceTag<T>>
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
		static ServiceContainer& instance()
		{
			static ServiceContainer container;
			return container;
		}
	};

	class SingletonContainer
	{
	private:
		std::unordered_map<TypeIndex, std::shared_ptr<void>> instances;
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

		template<typename T, typename... Args>
			requires std::derived_from<T, SingletonTag<T>>
		void register_singleton(Args&&... args)
		{
			register_instance<T>(std::make_shared<T>(std::forward<Args>(args)...));
		}

		// polymorphic only
		template<typename T, typename U, typename... Args>
			requires std::derived_from<T, SingletonTag<T>> and std::derived_from<U, T>
		void register_singleton_polymorphic(Args&&... args)
		{
			register_instance<T>(std::make_shared<U>(std::forward<Args>(args)...));
		}
		
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
			static SingletonContainer inst;
			return inst;
		}
	};

	// have to inject new instances per-thread starting at object graph root container to avoid races
	class ThreadScope
	{
	private:
		// type erasure is fine since shared ptr stores type-specific deleter
		std::unordered_map<TypeIndex, std::shared_ptr<void>> instances;

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

	// mixin, e.g., MyClass : public InjectService<MyService> { } ... using_service<T>().some_method()
	// NOTE: requires a complete type--include prerequisite headers before using!
	template<typename... Ts>
		requires (std::derived_from<Ts, ServiceTag<Ts>> && ...)
	class InjectService
	{
	private:
		std::tuple<std::shared_ptr<Ts>...> services;

	public:
		// resolve a specific thread-local service instance. requires static complete type parameter
		template<typename T>
		std::shared_ptr<T> using_service() const
		{
			return std::get<std::shared_ptr<T>>(services);
		}

	protected:
		InjectService()
			: services{ (using_thread().get<Ts>())... }
		{
		}
	};

	// mixin, e.g., MyClass : public InjectSingleton<MyService> { } ... using_global_service<T>().some_method()
	// NOTE: requires a complete type--include prerequisite headers before using!
	template<typename... Ts>
		requires (std::derived_from<Ts, SingletonTag<Ts>> && ...)
	class InjectSingleton
	{
	private:
		std::tuple<std::shared_ptr<Ts>...> services;

	public:
		// resolve a specific global singleton instance. requires static complete type parameter
		template<typename T>
		std::shared_ptr<T> using_singleton() const
		{
			return std::get<std::shared_ptr<T>>(services);
		}

	public:
		InjectSingleton()
			: services{ (SingletonContainer::instance().get<Ts>())... }
		{
		}
	};

	// Autoregistration is kinda dangerous so not sure if the following is ready for deployment yet

	// template<typename T, bool AutoRegisterB, typename... Args>
	// struct ServiceTag
	// {
	// 	// this will be automatically called if there are no args to pass
	// 	ServiceTag(Args&&... args)
	// 	{
	// 		if constexpr (AutoRegisterB)
	// 		{
	// 			ServiceContainer::instance().register_factory<T>([args... = std::forward<Args>(args)]() mutable
	// 			{
	// 				return std::make_shared<T>(std::forward<Args>(args)...);
	// 			});
	// 		}
	// 	}
	// };

	// template<typename T, bool AutoRegisterB, typename... Args>
	// struct SingletonTag
	// {
	// 	// this will be automatically called if there are no args to pass
	// 	SingletonTag(Args&&... args)
	// 	{
	// 		if constexpr (AutoRegisterB)
	// 		{
	// 			SingletonContainer::instance().register_singleton<T>(
	// 				std::forward<Args>(args)...)
	// 		}
	// 	}
	// };

#define REQUIRE_SAFE(x) using_service<x>()
//#define REQUIRE_UNSAFE(x) ServiceContainer::instance().get<x>()
#define REQUIRE_UNSAFE(x) using_thread().get<x>()

	// Denoted unsafe because the call could fail if the singleton was not registered
	// NOTE: DO NOT DEFERENCE THE SINGLETON POINTER BECAUSE IT MIGHT BE POLYMORPHIC!
#define USE_UNSAFE(x) SingletonContainer::instance().get<x>()
	// Denoted safe because the caller has already injected the singleton (so it would have failed already), so this call cannot fail
	// NOTE: not explicitly thread-safe, so use with caution!
#define USE_SAFE(x) using_singleton<x>()

#define RUN_SYNC(x, y) SingletonContainer::instance().run_synchronized<x>(y)

}

#endif
