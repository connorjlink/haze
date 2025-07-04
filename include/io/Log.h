#ifndef HAZE_LOG_H
#define HAZE_LOG_H

#include <error/ErrorType.h>

// Haze Log.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Log
	{
	private:
		static void print(const std::string&, const std::string&);

	public:
		static void raw(const std::string&);

	public:
		static void with_severity(const std::string&, ErrorType);

	public:
		static void output(const std::string&);
		static void info(const std::string&);
		static void warning(const std::string&);
		[[noreturn]]
		static void error(const std::string&);
		static void uncorrectable(const std::string&);
	};
}

#endif
