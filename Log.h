#ifndef HAZE_LOG_H
#define HAZE_LOG_H

#include "ErrorType.h"

#include <string_view>

namespace hz
{
	class Log
	{
	private:
		static void print(std::string_view, std::string_view);

	public:
		static void raw(std::string_view);

	public:
		static void with_severity(std::string_view, ErrorType);

	public:
		static void output(std::string_view);
		static void info(std::string_view);
		static void warning(std::string_view);
		[[noreturn]]
		static void error(std::string_view);
		static void uncorrectable(std::string_view);
	};
}

#endif //HAZE_LOG_H
