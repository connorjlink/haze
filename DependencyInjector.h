#ifndef HAZE_DEPENDENCYINJECTOR_H
#define HAZE_DEPENDENCYINJECTOR_H

// Haze DependencyInjector.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using type_index_id = void*;

	class type_index
	{
	private:
		type_index_id id;

	private:
		// NOTE: must be private to enforce explicit creation through thread-safe means
		type_index(type_index_id id)
			: id(id)
		{
		}

	public:
		auto operator<=>(const type_index& t) const = default;

		std::size_t hash_code() const
		{
			return std::hash<type_index_id>{}(id);
		}

		template<typename T>
		static constexpr type_index type_id_with_cvr()
		{
			static int id;
			return { static_cast<type_index_id>(&id) };
		};
	};

	// NOTE: this cannot be constexpr because it relies on statis instance data!!
	// MSVC GIVES TERRIBLE ERRORS IF YOU TRY
	template<typename T>
	type_index type_id()
	{
		using t = std::remove_cv_t<std::remove_reference_t<T>>;
		return type_index::type_id_with_cvr<t>();
	};
}

namespace std
{
	// IMPORTANT NOTE: must be defined before the usage in the container below! hence the separate namespace
	// std extension provides a clean means by which to interop with containers (like maps)
	template<>
	struct hash<hz::type_index>
	{
		size_t operator()(const hz::type_index& t) const
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
		std::unordered_map<type_index, std::function<std::shared_ptr<void>()>> _factories;
		std::mutex _mutex;

	public:
		template<typename T>
			requires std::derived_from<T, ServiceTag<T>>
		void register_factory(std::function<std::shared_ptr<T>()> factory)
		{
			std::scoped_lock lock{ _mutex };
			const auto id = type_id<T>();

			_factories[id] = [factory]()
			{
				return std::static_pointer_cast<void>(factory());
			};
		}

		template<typename T>
			requires std::derived_from<T, ServiceTag<T>>
		std::shared_ptr<T> create()
		{
			std::scoped_lock lock{ _mutex };
			const auto id = type_id<T>();

			if (!_factories.contains(id))
			{
				// no return
				std::println("unregistered factory type with entity id `{}`", id.hash_code());
				std::exit(1);
			}

			return std::static_pointer_cast<T>(_factories.at(id));
		}

	public:
		static ServiceContainer& instance()
		{
			static ServiceContainer inst;
			return inst;
		}
	};

	class SingletonContainer
	{
	private:
		std::unordered_map<type_index, std::shared_ptr<void>> _instances;
		std::mutex _mutex;

	public:
		template<typename T>
			requires std::derived_from<T, SingletonTag<T>>
		void register_instance(std::shared_ptr<T> instance)
		{
			std::scoped_lock lock{ _mutex };
			_instances[type_id<T>()] = std::static_pointer_cast<void>(instance);
		}

		template<typename T, typename... Args>
			requires std::derived_from<T, SingletonTag<T>>
		void register_singleton(Args&&... args)
		{
			register_instance<T>(std::make_shared<T>(std::forward<Args>(args)...));
		}
		
		// fetches an unsynchronized singleton handle. note
		// that accessing this handle is not thread-safe!
		template<typename T>
			requires std::derived_from<T, SingletonTag<T>>
		std::shared_ptr<T> get()
		{
			std::scoped_lock lock{ _mutex };
			const auto id = type_id<T>();

			if (!_instances.contains(id))
			{
				// no return
				std::println("unregistered global service type with entity id `{}`", id.hash_code());
				std::exit(1);
			}

			return std::static_pointer_cast<T>(_instances.at(id));
		}

		// Enforces a thread-safe synchronized call to a singleton service.
		// Stalls all threads competing for the same resource, so use sparingly!
		template<typename T, typename U>
			requires std::derived_from<T, SingletonTag<T>>
		U&& run_synchronized(std::function<U&&(std::shared_ptr<T>)> callable)
		{
			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			auto ptr = instance().get<T>();
			return callable(ptr);
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
		std::unordered_map<type_index, std::shared_ptr<void>> _instances;

	public:
		template<typename T>
			requires std::derived_from<T, ServiceTag<T>>
		std::shared_ptr<T> get()
		{
			const auto index = type_id<T>();

			if (_instances.contains(index))
			{
				const auto& existing_instance = _instances.at(index);
				return std::static_pointer_cast<T>(existing_instance);
			}

			const auto instance = ServiceContainer::instance().create<T>();
			_instances[index] = instance;
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
		std::tuple<std::shared_ptr<Ts>...> _services;

	public:
		// resolve a specific thread-local service instance. requires static complete type parameter
		template<typename T>
		std::shared_ptr<T> using_service() const
		{
			return std::get<T>(_services);
		}

	protected:
		InjectService()
			: _services{ (using_thread().get<Ts>())... }
		{
		}
	};

	// mixin, e.g., MyClass : public InjectSingleton<MyService> { } ... using_global_service<T>().some_method()
	template<typename... Ts>
		requires (std::derived_from<Ts, SingletonTag<Ts>> && ...)
	class InjectSingleton
	{
	private:
		std::tuple<std::shared_ptr<Ts>...> _services;

	public:
		// resolve a specific global singleton instance. requires static complete type parameter
		template<typename T>
		std::shared_ptr<T> using_singleton() const
		{
			return std::get<T>(_services);
		}

	public:
		InjectSingleton()
			: _services{ (SingletonContainer::instance().get<Ts>())... }
		{
		}
	};

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
