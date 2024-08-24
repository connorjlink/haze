#ifndef HAZE_ERRORREPORTER_H
#define HAZE_ERRORREPORTER_H

#include <string>
#include <stack>

#include "ErrorContext.h"
#include "Token.h"

namespace hz
{
	class ErrorReporter
	{
	private:
		static constexpr auto MAX_ERRORS = 5;

	private:
		bool _had_error;

	private:
		std::stack<ErrorContext> _open_contexts;
		std::vector<std::string> _closed_contexts;

	private:
		void validate_context();

	public:
		void open_context(std::string, std::string);
		void close_context();

	public:
		std::string generate_report();

	public:
		void post_information(std::string, Token);
		void post_warning(std::string, Token);
		void post_error(std::string, Token);
		[[noreturn]]
		void post_uncorrectable(std::string, Token);

	public:
		ErrorReporter()
		{
			_had_error = false;
		}

	};

	extern ErrorReporter* _error_reporter;
}

#endif 
