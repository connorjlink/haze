#ifndef HAZE_ERRORREPORTER_H
#define HAZE_ERRORREPORTER_H

#include "ErrorContext.h"
#include "Token.h"

#include <string>
#include <unordered_map>
#include <utility>
#include <list>
#include <stack>

// Haze ErrorReporter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ErrorReporter
	{
	private:
		static constexpr auto MAX_ERRORS = 5;

	public:
		bool _had_error;

	private:
		std::unordered_map<std::string, std::list<ErrorContext>> _open_contexts;
		std::vector<std::tuple<ErrorContext*, std::string>> _closed_contexts;

	private:
		std::stack<ErrorContext*> _active_contexts;
		std::stack<std::string> _active_files;

	public:
		void close_all_contexts();

	public:
		void open_context(const std::string&, const std::string&);
		void close_context();

	public:
		std::string generate_report();

	public:
		void post_information(const std::string&, Token);
		void post_warning(const std::string&, Token);
		void post_error(const std::string&, Token);
		[[noreturn]]
		void post_uncorrectable(const std::string&, Token);

	public:
		ErrorReporter()
		{
			_had_error = false;

			_open_contexts = {};
			_closed_contexts = {};

			_active_contexts = {};
			_active_files = {};
		}

	};

	extern ErrorReporter* _error_reporter;
}

#endif 
