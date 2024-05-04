#ifndef HAZE_LOG_H
#define HAZE_LOG_H

#include <iostream>
#include <fmt/format.h>

namespace hz
{
    class Log
    {
    private:
        static constexpr std::ostream& stream = std::cerr;

    private:
        static void print(std::string_view label, std::string_view msg)
        {
            stream << fmt::format("{}: {}\n", label, msg);
        }

    public:
        static void raw(std::string_view msg)
        {
            stream << msg;
        }

    public:
        static void info(std::string_view msg)
        {
            print("Information", msg);
        }

        [[noreturn]]
        static void error(std::string_view msg)
        {
            print("Error", msg);
            std::exit(EXIT_FAILURE);
        }

        static void warning(std::string_view msg)
        {
            print("Warning", msg);
        }
    };
}

#endif //HAZE_LOG_H
