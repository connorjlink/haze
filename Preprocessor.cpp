#include "Preprocessor.h"
#include "Log.h"

#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <format>
#include <filesystem>
#include <ranges>

namespace
{
    std::vector<std::string> split(std::string text, char delimiter)
    {
        return text
            | std::ranges::views::split(delimiter)
            | std::ranges::views::transform([](auto&& str)
                { return std::string_view(&*str.begin(), std::ranges::distance(str)); })
            | std::ranges::to<std::vector<std::string>>();
    }

    void replace(std::string& str, const std::string& from, const std::string& to)
    {
        if (from.empty())
        {
            return;
        }

        std::size_t start_pos = 0;

        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    constexpr auto ws = " \t\n\r";

    // Thanks https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring :)
    // trim from end of string (right)
    inline std::string& rtrim(std::string& s, const char* t = ws)
    {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    // trim from beginning of string (left)
    inline std::string& ltrim(std::string& s, const char* t = ws)
    {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

    // trim from both ends of string (right then left)
    inline std::string& trim(std::string& s, const char* t = ws)
    {
        return ltrim(rtrim(s, t), t);
    }
}

namespace hz
{
    void Preprocessor::preprocess_include()
    {
        std::regex rx_incl(R"(^(\.include \"(.*)\"\s*((;.*$)?|$)))", std::regex::optimize);
        std::smatch matches;

        while (std::regex_search(code, matches, rx_incl))
        {
            std::string filepath = matches[2];

            if (filepath == parent_filepath)
            {
                Log::error(std::format("file include recursion of {} is unsupported", filepath));
            }

            std::ifstream file(filepath);

            if (!file.good())
            {
                Log::error(std::format("could not open file {} for reading", filepath));
            }

            const auto size = std::filesystem::file_size(filepath); //bytes
            std::string buffer(size, '\0');
            file.read(buffer.data(), static_cast<std::streamsize>(size));

            replace(code, matches[1], buffer);
        }
    }

    void Preprocessor::preprocess_macro_definition()
    {
        std::regex rx_mcro(R"((\.macro ([a-zA-Z_][a-zA-Z0-0_]*) = \((([a-zA-Z])(, ?[a-zA-Z])*)?\)\:\s*\{([^\}]*)\}))", std::regex::optimize);
        std::smatch matches;

        while (std::regex_search(code, matches, rx_mcro))
        {
            std::string name = matches[2];
            std::string args = matches[3];

            args.erase(std::remove_if(args.begin(), args.end(), isspace), args.end());
            const auto args_delim = split(args, ',');

            std::string mcode = matches[6];
            trim(mcode);

            Macro macro{ name, args_delim, mcode };

            if (std::find_if(defined_macros.begin(), defined_macros.end(), [&](auto m)
                {
                    return (m.name == macro.name);
                }) != std::end(defined_macros))
            {
                Log::error(std::format("macro {} is multiply defined", macro.name));
            }

            defined_macros.emplace_back(macro);

            replace(code, matches[1], "");
        }
    }

    void Preprocessor::preprocess_macro_invokation()
    {
        std::regex rx_invk(R"((\$([a-zA-Z_][a-zA-Z0-9_]*)\(([^\,\)]*\s*(\,[^\,\)]*)*)?\)))", std::regex::optimize);
        std::smatch matches;

        while (std::regex_search(code, matches, rx_invk))
        {
            std::string name = matches[2];
            std::string args = matches[3];

            args.erase(std::remove_if(args.begin(), args.end(), isspace), args.end());
            const auto args_delim = split(args, ',');

            Macro macro{ name, args_delim, "" }, defined_macro;

            if (std::find_if(defined_macros.begin(), defined_macros.end(), [&](auto m)
                {
                    if (m.name == macro.name)
                    {
                        defined_macro = m;
                        return true;
                    }

                    return false;
                }) == defined_macros.end())
            {
                Log::error(std::format("macro {} is undefined", macro.name));
            }

            if (defined_macro.args.size() != args_delim.size())
            {
                Log::error(std::format("invokation of macro {} had {} arguments but expected {}",
                    defined_macro.name, args_delim.size(), defined_macro.args.size()));
            }

            if (!args_delim.empty())
            {
                std::string copy = defined_macro.code;

                // FIXME: implement this formatting thing properly (should remove extra new lines)
                /*if (copy[0] == '\n')

                if (copy[0] == '\n' &&
                    copy[copy.length()] == '\n')
                {

                }*/

                for (auto i = 0; i < args_delim.size(); ++i)
                {
                    replace(copy, std::string("[") + defined_macro.args[i] + std::string("]"), args_delim[i]);
                }

                replace(code, matches[1], copy);
            }
        }
    }

    std::string Preprocessor::preprocess()
    {
        preprocess_include();
        preprocess_macro_definition();
        preprocess_macro_invokation();
        return code;
    }
}
