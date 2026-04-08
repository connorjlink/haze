#ifndef HAZE_TRACKING_H
#define HAZE_TRACKING_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze Tracking.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using TrackingId = std::size_t;
	using TrackingTimePoint = std::chrono::time_point<std::chrono::system_clock>;

	static constexpr inline TrackingTimePoint NEVER = TrackingTimePoint::min();

#ifndef NDEBUG
	static constexpr inline bool ENABLE_TRACKING = true;
#else
	static constexpr inline bool ENABLE_TRACKING = false;
#endif

	TrackingTimePoint system_timestamp(void);

	class Trackable;

	class Tracker
		: public SingletonTag<Tracker>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		struct TrackingInformation
		{
		public:
			// for tracking memory leaks
			std::size_t number_created;
			std::size_t number_retired;
			std::size_t number_deleted;

			// specifies the first ever time this entity was created
			TrackingTimePoint created;
			// specifies the most recent transaction for modified and retired
			TrackingTimePoint modified;
			TrackingTimePoint retired;
			// specifies the last ever time this entity was deleted
			TrackingTimePoint deleted;

			// false if the entity is retired or deleted
			bool is_active;

		public:
			TrackingInformation(std::size_t number_created, TrackingTimePoint created, bool is_active) 
				: number_created{ number_created }, created{ created }, is_active{ is_active }
			{
				number_retired = 0;
				number_deleted = 0;

				modified = NEVER;
				retired = NEVER;
				deleted = NEVER;
			}
		};

	private:
		std::unordered_map<TrackingId, TrackingInformation> _database;

	private:
		bool validate_exists(TrackingId id)
		{
			if (!_database.contains(id))
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"invalid attachment to nonexistent entity tracking instance `{}`", id), NULL_TOKEN);
				return false;
			}

			return true;
		}

		template<typename T>
			requires std::derived_from<T, Trackable>
		bool validate_notnull(T* entity)
		{
			if (entity == nullptr)
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"invalid nullish entity tracking instance"), NULL_TOKEN);
				return false;
			}

			return true;
		}

		template<typename T>
			requires std::derived_from<T, Trackable>
		bool strengthen_ensure(T* entity)
		{
			if (!validate_notnull(entity))
			{
				return false;
			}

			if (!validate_exists(entity->_id))
			{
				return false;
			}

			return true;
		}

	public:
		template<typename T>
			requires std::derived_from<T, Trackable>
		//and requires { std::declval<T*>()->_id; } // not necessary since the trackable base will grant this
		bool notify_created(T* entity)
		{
			if (!validate_notnull(entity))
			{
				return false;
			}

			// NOTE: since the tracking indexes by type, it must allow duplicates to multiple instantiations
			/*if (_database.contains(entity->_id))
			{
				_error_reporter->post_error(std::format(
					"invalid duplicate tracking instance of entity with id `{}`", entity._id), NULL_TOKEN);
				return false;
			}*/

			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			if (!_database.contains(entity->_id))
			{
				// 1 created @ now, active
				_database.try_emplace(entity->_id, 1, system_timestamp(), true);
			}

			else
			{
				auto& entry = _database.at(entity->_id);
				entry.number_created++;
				// don't update the timestamp to keep the tracked time from the very first instantiation
				//entry.created = system_timestamp();
				entry.is_active = true;
			}

			return true;
		}

		template<typename T>
			requires std::derived_from<T, Trackable>
		bool notify_modified(T* entity)
		{
			if (!strengthen_ensure(entity))
			{
				return false;
			}

			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			auto& entry = _database.at(entity->_id);
			entry.number_modified++;
			entry.modified = system_timestamp();
			// active flag always reflects the results of the most recent transaction
			entry.is_active = true;

			return true;
		}

		template<typename T>
			requires std::derived_from<T, Trackable>
		bool notify_retired(T* entity)
		{
			if (!strengthen_ensure(entity))
			{
				return false;
			}

			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			auto& entry = _database.at(entity->_id);
			entry.number_retired++;
			entry.retired = system_timestamp();
			entry.is_active = false;

			return true;
		}

		template<typename T>
			requires std::derived_from<T, Trackable>
		bool notify_deleted(T* entity)
		{
			if (!strengthen_ensure(entity))
			{
				return false;
			}

			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			auto& entry = _database.at(entity->_id);
			entry.number_deleted++;
			entry.is_active = false;

			if (entry.number_created == entry.number_deleted)
			{
				// only timestamp the last entity to get deleted
				entry.deleted = system_timestamp();
			}

			return true;
		}

	public:
		// even though these processes will be slow, this has to take a copy to prevent invalidating the database
		// in case the functions get called at a time other than program exit

		std::vector<decltype(_database)::value_type> get_tracking_information(void) const
		{
			std::vector<decltype(_database)::value_type> result{};
			result.reserve(_database.size());

			for (auto& kv : _database)
			{
				result.emplace_back(kv);
			}

			return result;
		}

		std::vector<TrackingInformation> scan_stale_entities(void) const
		{
			std::vector<TrackingInformation> result{};

			for (const auto& [key, value] : _database)
			{
				if (value.number_retired > value.number_deleted && value.is_active)
				{
					result.emplace_back(value);
				}
			}
			return result;
		}

		std::vector<TrackingInformation> scan_memory_leaks(void) const
		{
			std::vector<TrackingInformation> result{};

			for (const auto& [key, value] : _database)
			{
				if (value.number_created > value.number_deleted)
				{
					result.emplace_back(value);
				}
			}

			return result;
		}
	};

	// provides some generic machinery for more detailed instrumentation by the compiler driver
	// not templated because C++23 deducing this does not require it
	class Trackable 
		: public InjectSingleton<Tracker>
	{
	private:
		friend Tracker;
		TrackingId _id;
		bool _is_enabled;

	private:
		static TrackingId generate_id(void)
		{
			// thread-safe! generated per Trackable type template instantiation
			static std::atomic<TrackingId> index{ 0 };
			return ++index;
		}

	protected:
		// manually notify the change tracker that the object has been modified to forcibly flush its changes
		template<typename Self>
		void commit(this Self& self)
		{
			if (_is_enabled)
			{
				if constexpr (requires { self.update(); })
				{
					self.update();
				}
				USE_SAFE(Tracker)->notify_modified(this);
			}
		}

		// manually notify the change tracker that the object is done being used but will remain alive and allocated longer
		template<typename Self>
		void retire(this Self& self)
		{
			if (_is_enabled)
			{
				USE_SAFE(Tracker)->notify_retired(this);
			}
		}

	public:
		// dynamically enable or disable entity tracking for this instance
		Trackable(bool is_enabled = true)
			: _is_enabled{ is_enabled }
		{
			if (_is_enabled)
			{
				_id = generate_id();
				USE_SAFE(Tracker)->notify_created(this);
			}
		}

		~Trackable()
		{
			if (_is_enabled)
			{
				USE_SAFE(Tracker)->notify_deleted(this);
			}
		}
	};
}

#endif
