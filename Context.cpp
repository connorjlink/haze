#include "Context.h"
#include "ErrorReporter.h"
#include "ErrorState.h"

import std;

// Haze Context.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Context::define_variable(std::string identifier, variable_t value)
	{
		_variables[identifier] = value;
	}

	const decltype(Context::_variables)& Context::variables() const
	{
		return _variables;
	}

	void Context::define_function(Function* function)
	{
		_functions.emplace_back(function);
	}

	const decltype(Context::_functions)& Context::functions() const
	{
		return _functions;
	}

	void Context::push_return(return_t value)
	{
		_returns.emplace(value);
	}

	return_t Context::pop_return()
	{
		const auto value = _returns.top();
		_returns.pop();
		return value;
	}

	void Context::push_arguments(arguments_t arguments)
	{
		_arguments.emplace(arguments);
	}

	arguments_t Context::pop_arguments()
	{
		const auto arguments = _arguments.top();
		_arguments.pop();
		return arguments;
	}

	void Context::hook()
	{
		//_hook.hook();
	}

	void Context::unhook()
	{
		//_hook.unhook();
	}

	void Context::print(const std::string& message)
	{
		_error_reporter->post_information(message, NULL_TOKEN);
	}

	void Context::exit_program(variable_t value)
	{
		switch (value.index())
		{
			case 0: 
			{
				auto int_value = std::get<0>(value);
				_error_reporter->post_information(std::format("exited with code `{}`", int_value), NULL_TOKEN);
			} break;

			case 1:
			{
				auto string_value = std::get<1>(value);
				_error_reporter->post_information(std::format("exited with message `{}`", string_value), NULL_TOKEN);
			} break;

			default:
			{
				_error_reporter->post_error("invalid type for `exit`", NULL_TOKEN);
			} break;
		}

		std::longjmp(_jump_state, 1);
	}
}