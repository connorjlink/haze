#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

#include <string>
#include <vector>
#include <stack>
#include <variant>
#include <unordered_map>

#include "Hook.h"

namespace hz
{	
	// Required to avoid a dependency between Function and Context
	class Function;

	// Required to avoid a dependency between Expression and Context
	class Expression;


	//using variable_t = int;
	using variable_t = std::variant<int, std::string>;

	// TODO: scope these types further in (within Context?)
	using arguments_t = std::vector<Expression*>;
	using return_t = variable_t;

	class Context
	{
	private:
		std::unordered_map<std::string, variable_t> _variables;
		std::vector<Function*> _functions;

		std::stack<return_t> _returns;
		std::stack<arguments_t> _arguments;

	private:
		Hook _hook;

	public:
		void define_variable(std::string, variable_t);
		const decltype(_variables)& variables() const;

		void define_function(Function*);
		const decltype(_functions)& functions() const;

	public:
		void push_return(return_t);
		return_t pop_return();

	public:
		void push_arguments(arguments_t);
		arguments_t pop_arguments();

	public:
		void hook();
		void unhook();

	public:
		void print(const std::string& message);

	public:
		Context()
			: _variables{}, _functions{}, _returns{}, _arguments{}
		{
		}

	};

	extern Context* _context;
}

#endif
