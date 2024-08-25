#include "CommandLineParser.h"
#include "JobManager.h"
#include "ErrorReporter.h"

#include <vector>
#include <ranges>

namespace
{
	std::vector<std::string> split(std::string text, std::string delimiter)
	{
		return text
			| std::ranges::views::split(delimiter)
			| std::ranges::views::transform([](auto&& str) 
				{ return std::string_view(&*str.begin(), std::ranges::distance(str)); })
			| std::ranges::to<std::vector<std::string>>();
	}

	std::vector<std::string> convert(int argc, char** argv)
	{
		std::vector<std::string> arguments{};
		arguments.reserve(argc);

		for (auto i = 0; i < argc; i++)
		{
			arguments.emplace_back(argv[i]);
		}

		return arguments;
	}
}

namespace hz
{
	CommandLineOptions parse(int argc, char** argv)
	{
		static constexpr auto TASK = "identifying arguments";

		const auto identify_task = _job_manager->begin_job(TASK);
		const auto arguments = convert(argc, argv);
		
		_error_reporter->open_context("command line", TASK);

		for (auto& argument : arguments)
		{
			// parsing a command-line option
			if (argument.starts_with("--"))
			{
				const auto argument_unprefixed = argument.substr(2);
				const auto argument_split = ::split(argument_unprefixed, "=");

				if (argument_split.size() != 2)
				{
					_error_reporter->post_warning(std::format("unrecognized option format {}", argument), NULL_TOKEN);
					continue;
				}

				const auto& option_string = argument_split[0];

				if (!_option_map.contains(option_string))
				{
					_error_reporter->post_warning(std::format("unrecognized option {}", option_string), NULL_TOKEN);
					continue;
				}

				const auto option = _option_map.at(option_string);
				const auto& value = argument_split[1];

				using enum OptionType;
				switch (option)
				{
					case ARCHITECTURE:
					{
						if (!_architecture_map.contains(value))
						{
							_error_reporter->post_warning(std::format("unrecognized architecture type {}", value), NULL_TOKEN);
							continue;
						}

						_options->_architecture = _architecture_map.at(value);
					} break;

					case VERBOSITY:
					{
						if (!_verbosity_map.contains(value))
						{
							_error_reporter->post_warning(std::format("unrecognized verbosity type {}", value), NULL_TOKEN);
							continue;
						}

						_options->_verbosity = _verbosity_map.at(value);
					} break;

					case OPTIMIZATION:
					{
						if (!_optimization_map.contains(value))
						{
							_error_reporter->post_warning(std::format("unrecognized optimization type {}", value), NULL_TOKEN);
							continue;
						}

						auto flag = _optimization_map.at(value);
						_options->_optimization |= flag;
					} break;
				}
			}

			// parsing a source code filepath
			else
			{

			}
		}

		_error_reporter->close_context();
		_job_manager->end_job(identify_task);
	}
}