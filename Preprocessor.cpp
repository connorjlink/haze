#include "Preprocessor.h"
#include "Log.h"

#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <format>
#include <filesystem>

namespace
{
    std::vector<std::string> split(const std::string& text, char delim)
    {
        std::string line;
        std::vector<std::string> vec;
        std::stringstream ss(text);

        while (std::getline(ss, line, delim))
        {
            vec.emplace_back(line);
        }

        return vec;
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
}

namespace hz
{
    void Preprocessor::preprocess_include()
    {
        std::regex rx_incl(R"((\.include \"(.*)\"\s*((;.*$)?|$)))");
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

    // TODO: finish this before too long
    void Preprocessor::preprocess_definition()
    {
        std::regex rx_pdef(R"()");
        std::smatch matches;
    }

    void Preprocessor::preprocess_macro_definition()
    {
        std::regex rx_mcro(R"((\.macro ([a-zA-Z_][a-zA-Z0-0_]*) = \((([a-zA-Z])(, ?[a-zA-Z])*)?\)\:\s*\{([^\}]*)\}))");
        std::smatch matches;

        while (std::regex_search(code, matches, rx_mcro))
        {
            std::string name = matches[2];
            std::string args = matches[3];

            args.erase(std::remove_if(args.begin(), args.end(), isspace), args.end());
            const auto args_delim = split(args, ',');

            std::string mcode = matches[6];

            Macro macro{ name, args_delim, mcode };

            if (std::find_if(defined_macros.begin(), defined_macros.end(), [&](auto m)
                {
                    return (m.name == macro.name);
                }) == std::end(defined_macros))
            {
                Log::error(std::format("macro {} is multiply defined", macro.name));
            }

            defined_macros.emplace_back(macro);

            replace(code, matches[1], "");
        }
    }

    void Preprocessor::preprocess_macro_invokation()
    {
        std::regex rx_invk(R"((\$([a-zA-Z_][a-zA-Z0-9_]*)\(([^\,\)]*\s*(\,[^\,\)]*)*)?\)))");
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
                }) == std::end(defined_macros))
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

                for (auto i = 0u; i < args_delim.size(); ++i)
                {
                    replace(copy, std::string("[") + defined_macro.args[i] + std::string("]"), args_delim[i]);
                }

                replace(code, matches[1], copy);
            }
        }
    }

    void Preprocessor::preprocess()
    {
        preprocess_include();
        preprocess_macro_definition();
        preprocess_macro_invokation();
        preprocess_definition();
    }
}
