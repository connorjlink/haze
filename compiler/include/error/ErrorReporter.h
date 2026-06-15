#ifndef HAZE_ERRORREPORTER_H
#define HAZE_ERRORREPORTER_H

#include <data/DependencyInjector.h>
#include <error/ErrorContext.h>
#include <toolchain/models/Token.h>

// Haze ErrorReporter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct ErrorReporter 
		: public SingletonTag<ErrorReporter>
	{
	private:
		static constexpr auto MAX_ERRORS = 5;

	private:
		std::atomic<std::int32_t> error_count;

	private:
		void validate_error_count()
		{
			if (error_count > MAX_ERRORS)
			{
				post_uncorrectable(std::format("compilation stopped because of excessive errors ({})", MAX_ERRORS), NULL_TOKEN);
			}
		}

	public:
		bool had_error() const
		{
			return error_count > 0;
		}

	private:
		std::mutex mutex;

	private:
		std::unordered_map<std::thread::id, std::unordered_map<std::string, std::list<ErrorContext>>> open_frames;
		std::unordered_map<std::thread::id, std::vector<ErrorFrame>> closed_frames;

	private:
		std::unordered_map<std::thread::id, std::stack<ErrorFrame>> active_frames;

	private:
		void close_these_contexts(std::thread::id);

	public:
		void close_these_contexts();
		void close_all_contexts();

	public:
		std::size_t get_context_count();
		ErrorFrame open_context(const std::filesystem::path&, std::string_view);
		void close_context();

	public:
		std::string generate_report();

	public:
		void post_information(const std::string&, const Token&);
		void post_warning(const std::string&, const Token&);
		void post_error(const std::string&, const Token&);
		[[noreturn]]
		void post_uncorrectable(const std::string&, const Token&);

	public:
		void post_information(ErrorContext*, const std::filesystem::path&, const std::string&, const Token&);
		void post_warning(ErrorContext*, const std::filesystem::path&, const std::string&, const Token&);
		void post_error(ErrorContext*, const std::filesystem::path&, const std::string&, const Token&);
		[[noreturn]]
		void post_uncorrectable(ErrorContext*, const std::filesystem::path&, const std::string&, const Token&);

	public:
		ErrorReporter()
		{
			error_count = 0;

			open_frames = {};
			closed_frames = {};

			active_frames = {};
		}
	};
}

#endif 
