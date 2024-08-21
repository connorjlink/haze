#include "Context.h"
#include "Log.h"

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

	void Context::print(const std::string& message)
	{
		Log::output(message);
	}
}