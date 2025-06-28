#ifndef HAZE_GATEWAY_H
#define HAZE_GATEWAY_H

#include "DependencyInjector.h"

// Haze Tracking.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using tracking_id = std::size_t;
	using tracking_time_point = std::chrono::time_point<std::chrono::system_clock>;

	tracking_time_point system_timestamp(void)
	{
		return std::chrono::system_clock::now();
	}

	class TrackingService;

	// provides some generic machinery for more detailed instrumentation by the compiler driver
	template<typename T>
	class Trackable : public Inject<TrackingService>
	{
	private:
		friend TrackingService;
		tracking_id _id;
		const bool _is_enabled;

	private:
		static tracking_id generate_id(void)
		{
			static std::atomic<tracking_id> index{ 0 };
			return ++counter;
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
				get_service<TrackingService>().notify_modified(this);
			}
		}

		// manually notify the change tracker that the object is done being used but will remain alive and allocated longer
		template<typename Self>
		void retire(this Self self)
		{
			if (is_enabled)
			{
				get_service<TrackingService>().notify_retired(this);
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
				get_service<TrackingService>().notify_created(this);
			}
		}

		~Trackable()
		{
			if (_is_enabled)
			{
				get_service<TrackingService>().notify_deleted(this);
			}
		}
	};

	class TrackingService
	{
	private:
		struct TrackingInformation
		{
		public:
			// for tracking memory leaks
			std::size_t number_created;
			std::size_t number_deleted;

		public:
#error TODO FIGURE OUT IF TIMESTAMPING THIS MAKES ANY SENSE SINCE THIS IS TRACKING BY TYPE, NOT BY INSTANCE
			tracking_time_point created;
			tracking_time_point modified;
			tracking_time_point retired;
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
				_error_reporter->post_error(std::format(
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
				_error_reporter->post_error(std::format(
					"invalid nullish entity tracking instance"), NULL_TOKEN);
				return false;
			}

			return true;
		}

	public:
		template<typename T>
			requires std::is_base_of_v<T, Trackable<T>>
		bool notify_created(T* entity)
		{
			if (!validate_notnull(entity))
			{
				return false;
			}

			if (_database.contains(entity->_id))
			{
				_error_reporter->post_error(std::format(
					"invalid duplicate tracking instance of entity with id `{}`", entity._id), NULL_TOKEN);
				return false;
			}

			TrackingInformation entry
			{
				.created = std::chrono::system_clock::now(),
				// let others default initialize since they do not 
				.is_active = true,
			};

			_database.emplace(entity->_id, entry);
		}

		template<typename T>
			requires std::is_base_of_v<T, Trackable<T>>
		bool notify_modified(T* entity)
		{
			if (!validate_notnull(entity))
			{
				return false;
			}

			if (!validate_existing(entity->_id))
			{
				return false;
			}

			auto& entry = _database.at(entity->_id);
			entry.modified = system_timestamp();
			// active flag always reflects the results of the most recent transaction
			entry.is_active = true;
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
}

#endif
