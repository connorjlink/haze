#ifndef HAZE_TRACKING_H
#define HAZE_TRACKING_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/Constants.h>

// Haze Tracking.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using TrackingId = std::size_t;
	using TrackingTimePoint = std::chrono::time_point<std::chrono::system_clock>;

	inline constexpr TrackingTimePoint NEVER = TrackingTimePoint::min();


	TrackingTimePoint system_timestamp();


	struct Tracker
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
				: number_created{ number_created }
				, created{ created }
				, is_active{ is_active }
				, number_retired{ 0 }
				, number_deleted{ 0 }
				, modified{ NEVER }
				, retired{ NEVER }
				, deleted{ NEVER } 
			{
			}
		};

	private:
		std::unordered_map<TrackingId, TrackingInformation> database;

	private:
		bool validate_exists(TrackingId id)
		{
			if (!database.contains(id))
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"invalid attachment to nonexistent entity tracking instance `{}`", id), NULL_TOKEN);
				return false;
			}

			return true;
		}

		template<typename T>
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
		bool strengthen_ensure(T* entity)
		{
			if (!validate_notnull(entity))
			{
				return false;
			}

			if (!validate_exists(entity->id))
			{
				return false;
			}

			return true;
		}

	public:
		template<typename T>
		bool notify_created(T* entity)
		{
			if (!validate_notnull(entity))
			{
				return false;
			}

			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			if (!database.contains(entity->id))
			{
				// 1 created @ now, active
				database.try_emplace(entity->id, 1, system_timestamp(), true);
			}
			else
			{
				auto& entry = database.at(entity->id);
				entry.number_created++;
				// don't update the timestamp to keep the tracked time from the very first instantiation
				//entry.created = system_timestamp();
				entry.is_active = true;
			}

			return true;
		}

		template<typename T>
		bool notify_modified(T* entity)
		{
			if (!strengthen_ensure(entity))
			{
				return false;
			}

			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			auto& entry = database.at(entity->id);
			entry.number_modified++;
			entry.modified = system_timestamp();
			// active flag always reflects the results of the most recent transaction
			entry.is_active = true;

			return true;
		}

		template<typename T>
		bool notify_retired(T* entity)
		{
			if (!strengthen_ensure(entity))
			{
				return false;
			}

			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			auto& entry = database.at(entity->id);
			entry.number_retired++;
			entry.retired = system_timestamp();
			entry.is_active = false;

			return true;
		}

		template<typename T>
		bool notify_deleted(T* entity)
		{
			if (!strengthen_ensure(entity))
			{
				return false;
			}

			static std::mutex mutex;
			std::scoped_lock lock{ mutex };

			auto& entry = database.at(entity->id);
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

		std::vector<decltype(database)::value_type> get_tracking_information() const
		{
			auto result = std::vector<decltype(database)::value_type>{};
			result.reserve(database.size());

			for (auto& pair : database)
			{
				result.emplace_back(pair);
			}

			return result;
		}

		std::vector<TrackingInformation> scan_stale_entities() const
		{
			auto result = std::vector<TrackingInformation>{};

			for (const auto& [key, value] : database)
			{
				if (value.number_retired > value.number_deleted && value.is_active)
				{
					result.emplace_back(value);
				}
			}

			return result;
		}

		std::vector<TrackingInformation> scan_memory_leaks() const
		{
			auto result = std::vector<TrackingInformation>{};

			for (const auto& [key, value] : database)
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
	struct TrackableEnabled
		: public InjectSingleton<Tracker>
	{
	private:
		friend Tracker;
		TrackingId id;

	private:
		static TrackingId generate_id()
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
			if constexpr (requires { self.update(); })
			{
				self.update();
			}

			USE_SAFE(Tracker)->notify_modified(this);
		}

		// manually notify the change tracker that the object is done being used but will remain alive and allocated longer
		template<typename Self>
		void retire(this Self& self)
		{
			USE_SAFE(Tracker)->notify_retired(this);
		}

	public:
		TrackableEnabled()
		{
			id = generate_id();
			USE_SAFE(Tracker)->notify_created(this);
		}

		~TrackableEnabled()
		{
			USE_SAFE(Tracker)->notify_deleted(this);
		}
	};

	struct TrackableDisabled
	{
		// dummy zero-size struct
	};

	using Trackable = std::conditional_t
	<
		IS_DEBUG,
		TrackableEnabled,
		TrackableDisabled
	>;
}

#endif
