#ifndef HAZE_ERRORREPORTER_H
#define HAZE_ERRORREPORTER_H

#include <data/DependencyInjector.h>
#include <error/ErrorContext.h>
#include <toolchain/models/Token.h>

// Haze ErrorReporter.h
// (c) Connor J. Link. All Rights Reserved.

#pragma message("TODO: provide safe API for error reporter with thread synchonization!")

namespace hz
{
	class ErrorReporter :
		public SingletonTag<ErrorReporter>
	{
	private:
		static constexpr auto MAX_ERRORS = 5;

	private:
		std::int32_t _error_count;

	private:
		void validate_error_count()
		{
			if (_error_count > MAX_ERRORS)
			{
				post_uncorrectable(std::format("compilation stopped because of excessive errors ({})", MAX_ERRORS), NULL_TOKEN);
			}
		}

	public:
		bool had_error() const
		{
			return _error_count > 0;
		}

	private:
		std::unordered_map<std::string, std::list<ErrorContext>> _open_frames;
		std::vector<ErrorFrame> _closed_frames;

	private:
		std::stack<ErrorFrame> _active_frames;

	public:
		void close_all_contexts();

	public:
		std::size_t get_context_count();
		ErrorFrame open_context(const std::string&, const std::string&);
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
		void post_information(ErrorContext*, const std::string&, const std::string&, const Token&);
		void post_warning(ErrorContext*, const std::string&, const std::string&, const Token&);
		void post_error(ErrorContext*, const std::string&, const std::string&, const Token&);
		[[noreturn]]
		void post_uncorrectable(ErrorContext*, const std::string&, const std::string&, const Token&);

	public:
		ErrorReporter()
		{
			_error_count = 0;

			_open_frames = {};
			_closed_frames = {};

			_active_frames = {};
		}

	};
}

#endif 
