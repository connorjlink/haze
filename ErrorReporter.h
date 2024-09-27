#ifndef HAZE_ERRORREPORTER_H
#define HAZE_ERRORREPORTER_H

#include "ErrorContext.h"
#include "Token.h"
#include "Test.h"

// Haze ErrorReporter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ErrorReporter
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
		std::unordered_map<std::string, std::list<ErrorContext>> _open_contexts;
		std::vector<std::tuple<ErrorContext*, std::string>> _closed_contexts;

	private:
		std::stack<ErrorContext*> _active_contexts;
		std::stack<std::string> _active_files;

	public:
		void close_all_contexts();

	public:
		TestParameters open_context(const std::string&, const std::string&);
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

			_open_contexts = {};
			_closed_contexts = {};

			_active_contexts = {};
			_active_files = {};
		}

	};

	extern ErrorReporter* _error_reporter;
}

#endif 
