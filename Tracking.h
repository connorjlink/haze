#ifndef HAZE_TRACKING_H
#define HAZE_TRACKING_H

#include "ErrorReporter.h"
#include "DependencyInjector.h"

// Haze Tracking.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using tracking_id = std::size_t;
	using tracking_time_point = std::chrono::time_point<std::chrono::system_clock>;

#ifndef NDEBUG
	static constexpr inline bool ENABLE_TRACKING = true;
#else
	static constexpr inline bool ENABLE_TRACKING = false;
#endif

	tracking_time_point system_timestamp(void);

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

		public:
			// specifies the first ever time this entity was created
			tracking_time_point created;
			// specifies the most recent transaction for modified and retired
			tracking_time_point modified;
			tracking_time_point retired;
			// specifies the last ever time this entity was deleted
			tracking_time_point deleted;

		public:
			bool is_active;
		};

	private:
		std::unordered_map<tracking_id, TrackingInformation> _database;

	private:
		bool validate_existing(tracking_id id)
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

			if (!validate_existing(entity->_id))
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
				TrackingInformation entry
				{
					.number_created = 1,
					.created = system_timestamp(),
					// let others default initialize since they don't matter for now
					.is_active = true,
				};

				_database.emplace(entity->_id, entry);
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
		std::vector<TrackingInformation> scan_memory_leaks(void) const
		{
			std::vector<TrackingInformation> result{};

			for (const auto& [key, value] : _database)
			{
				if (value.number_created > value.number_deleted)
				{
					// even though this process will be slow, this has to take a copy to prevent invalidating the database
					// in case this function is called at a time other than program exit
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
		tracking_id _id;
		bool _is_enabled;

	private:
		static tracking_id generate_id(void)
		{
			// thread-safe! generated per Trackable type template instantiation
			static std::atomic<tracking_id> index{ 0 };
			return ++index;
		}

	protected:
		// manually notify the change tracker that the object has been modified to forcibly flush its changes
		template<typename Self>
		void commit(this Self self)
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
		void retire(this Self self)
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
