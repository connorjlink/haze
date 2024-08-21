#ifndef HAZE_LOG_H
#define HAZE_LOG_H

#include <iostream>
#include <format>

#define stream std::cerr

#define DISCARD (void) 

namespace hz
{
	class Log
	{
	private:
		static void print(std::string_view label, std::string_view message)
		{
			stream << std::format("{}: {}\n", label, message);
		}

	public:
		static void raw(std::string_view message)
		{
			stream << message;
		}

	public:
		static void output(std::string_view message)
		{
			print("Output", message);
		}

		static void info(std::string_view message)
		{
			print("Information", message);
		}

		[[noreturn]]
		static void error(std::string_view message)
		{
			print("Error", message);
			std::exit(EXIT_FAILURE);
		}

		static void warning(std::string_view message)
		{
			print("Warning", message);
		}
	};
}

#endif //HAZE_LOG_H
