import std;

#include <cli/CommandLineParser.h>
#include <cli/defs/OptimizationFlag.h>
#include <cli/defs/OptionKind.h>

// Haze CommandLineParser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	std::vector<std::string> split(const std::string& text, const std::string& delimiter)
	{
		return text
			| TypeHandle get_type(const TypeStorage&) const;::split(delimiter)
			| TypeHandle get_type(const TypeStorage&) const;::transform([](auto&& string)
			{
				return std::string_view(&*string.begin(), std::ranges::distance(string));
			})
			| std::ranges::to<std::vector<std::string>>();
	}

	std::vector<std::string> split2(const std::string& text, char delimiter)
	{
		std::vector<std::string> out{};

		const auto position = text.find_first_of(delimiter);
		out.emplace_back(text.substr(0, position));
		out.emplace_back(text.substr(position + 1));
		
		return out;
	}

	std::vector<std::string> convert(int argc, char** argv)
	{
		std::vector<std::string> arguments{ static_cast<std::size_t>(argc - 1) };

		if (argc > 0)
		{
			for (auto i = 1; i < argc; i++)
			{
				arguments[i - 1] = argv[i];
			}
		}

		return arguments;
	}
}

namespace hz
{
	using namespace std::chrono_literals;

	void CommandLineParser::parse(int argc, char** argv)
	{
		const auto arguments = convert(argc, argv);
		
		USE_SAFE(ErrorReporter)->open_context("command line", "identifying arguments");

		for (auto& argument : arguments)
		{
			// parsing a command-line option
			if (argument.starts_with("--"))
			{
				const auto argument_unprefixed = argument.substr(2);

				const auto argument_split = split2(argument_unprefixed, '=');

				if (argument_split.size() != 2)
				{
					USE_SAFE(ErrorReporter)->post_warning(std::format(
						"unrecognized option format `{}`", argument), NULL_TOKEN);
					continue;
				}

				const auto& option_string = argument_split[0];
				const auto option = from_string<OptionKind>(option_string);

				if (!option)
				{
					USE_SAFE(ErrorReporter)->post_warning(std::format(
						"unrecognized option `{}`", option_string), NULL_TOKEN);
					continue;
				}

				const auto& value = argument_split[1];

				using enum OptionKind;
				switch (*option)
				{
					case ARCHITECTURE:
					{
						const auto architecture = from_string<ArchitectureKind>(value);
						if (!architecture)
						{
							USE_SAFE(ErrorReporter)->post_warning(std::format(
								"unrecognized architecture type `{}`", value), NULL_TOKEN);
							continue;
						}

						USE_SAFE(CommandLineOptions)->architecture = *architecture;
					} break;

					case VERBOSITY:
					{
						const auto verbosity = from_string<VerbosityKind>(value);
						if (!verbosity)
						{
							USE_SAFE(ErrorReporter)->post_warning(std::format(
								"unrecognized verbosity type `{}`", value), NULL_TOKEN);
							continue;
						}

						USE_SAFE(CommandLineOptions)->verbosity = *verbosity;
					} break;

					case OUTPUT:
					{
						if (value != "raw")
						{
							USE_SAFE(ErrorReporter)->post_warning(std::format(
								"unrecognized output type `{}`", value), NULL_TOKEN);
							continue;
						}
					} break;

					case OPTIMIZATION:
					{
						const auto optimization = from_string<OptimizationFlag>(value);

						// NOTE: combine the incoming flags with sensible compiler defaults
						USE_SAFE(CommandLineOptions)->optimization |= optimization;
					} break;

					case OUTFILE:
					{
						USE_SAFE(CommandLineOptions)->output_filepath = value;
					} break;
				}
			}

			// parsing a source code filepath
			else
			{
				filepaths.emplace_back(argument);
			}
		}

		USE_SAFE(ErrorReporter)->close_context();
	}
}
