#ifndef HAZE_DEPENDENCYNIJECTOR_H
#define HAZE_DEPENDENCYNIJECTOR_H

#include "ErrorReporter.h"

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
		// NOTE: must be private to enable only explicit creation through thread-safe means
		type_index(type_index_id id)
			: id(id)
		{
		}

	public:
		bool operator<=>(const type_index& t) const = default;

		std::size_t hash_code() const 
		{ 
			return std::hash<decltype(id)>()(id);
		}

		template<typename T>
		friend constexpr type_index type_id()
		{
			using t = std::remove_cv_t<std::remove_reference_t<T>>;
			return type_id_with_cvr<t>();
		};

		template<typename T>
		friend constexpr type_index type_id_with_cvr()
		{
			static int id;
			return &id;
		};
	};

	class ServiceContainer
	{
	private:
		std::unordered_map<type_index, std::function<std::shared_ptr<void>()>> _factories;
		std::mutex _mutex;

	public:
		template<typename T>
		void register_factory(std::function<std::shared_ptr<T>()> factory)
		{
			std::scoped_lock lock{ _mutex };
			_factories[type_index(type_id<T>())] = [factory]()
			{
				return std::static_pointer_cast<void>(factory());
			};
		}

		template<typename T>
		std::shared_ptr<T> create()
		{
			constexpr auto id = type_id<T>();

			std::scoped_lock lock{ _mutex };
			if (!_factories.contains(id))
			{
				// no return
				_error_reporter->post_uncorrectable(std::format(
					"unregistered factory type", NULL_TOKEN));
			}

			auto provider = _factories.at(id);
			return std::static_pointer_cast<T>(provider);
		}

	public:
		static ServiceContainer& instance()
		{
			static ServiceContainer inst;
			return inst;
		}
	};

	// have to inject per-thread starting at object graph root container to avoid races
	class ThreadScope
	{
	public:
		template<typename T>
		std::shared_ptr<T> get()
		{
			auto it = instances_.find(std::type_index(typeid(T)));
			if (it != instances_.end())
				return std::static_pointer_cast<T>(it->second);

			auto instance = ServiceContainer::instance().create<T>();
			instances_[std::type_index(typeid(T))] = instance;
			return instance;
		}

	private:
		std::unordered_map<std::type_index, std::shared_ptr<void>> instances_;
	};

	// wtf
	inline ThreadScope& using_thread()
	{
		thread_local ThreadScope scope;
		return scope;
	}

	// mixin, e.g., MyClass : public Inject<MyService> { } ... get_service().some_method()
	template<typename T>
	class Inject
	{
	private:
		T& service;

	public:
		T& get_service(void) const
		{
			return service;
		}

	protected:
		Inject()
			: service{ *hz::using_thread().get<T>() }
		{
		}
	};
}

// std extension provides a clean means by which to interop with containers (like maps)
namespace std
{
	template<>
	struct hash<hz::type_index>
	{
		std::size_t operator()(const hz::type_index& t) const
		{
			return t.hash_code();
		}
	};
}

#endif
