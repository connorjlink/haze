import std;

#include <runtime/Context.h>
#include <utility/ExitProgramException.h>

// Haze Context.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Context::declare_variable(const std::string& identifier)
	{
		define_variable(identifier, nullptr);
	}

	void Context::define_variable(const std::string& identifier, Variable* value)
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

	void Context::push_return(Variable* value)
	{
		_returns.emplace(value);
	}

	Variable* Context::pop_return()
	{
		const auto value = _returns.top();
		_returns.pop();
		return value;
	}

	void Context::push_arguments(const std::vector<Expression*>& arguments)
	{
		_arguments.emplace(arguments);
	}

	std::vector<Expression*> Context::pop_arguments()
	{
		const auto arguments = _arguments.top();
		_arguments.pop();
		return arguments;
	}

	void Context::print(const std::string& message)
	{
		USE_SAFE(ErrorReporter)->post_information(message, NULL_TOKEN);
	}

	void Context::exit_program(Variable* value)
	{
		// not an error condition, just a quick escape hatch to main()
		throw ExitProgramException{ value->format() };
	}
}