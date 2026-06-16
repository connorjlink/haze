import std;

#include <runtime/Context.h>
#include <utility/Exception.h>

// Haze Context.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Context::enter_function(const std::vector<FunctionArgument>& arguments)
	{
		enter_function();

		for (const auto& argument : arguments)
		{
			define(argument.name, argument.value);
		}
	}

	void Context::print(std::string_view message)
	{
		USE_SAFE(ErrorReporter)->post_information(message, NULL_TOKEN);
	}

	void Context::exit_program(VariableHandle variable)
	{
		// not an error condition, just a quick escape hatch to main()
		throw ExitProgramException{ variable.format() };
	}
}