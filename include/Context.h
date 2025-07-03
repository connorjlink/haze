#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

//#include "Hook.h"
#include "Variable.h"
#include "ErrorReporter.h"
#include "DependencyInjector.h"

// Haze Context.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{	
	// Required
	class Function;
	class Expression;

	class Context
		: public SingletonTag<Context>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		std::unordered_map<std::string, Variable*> _variables;
		std::vector<Function*> _functions;

		std::stack<Variable*> _returns;
		std::stack<std::vector<Expression*>> _arguments;

	private:
		//Hook _hook;
		bool _executing = true;

	public:
		void declare_variable(const std::string&);
		void define_variable(const std::string&, Variable*);
		const decltype(_variables)& variables() const;

		void define_function(Function*);
		const decltype(_functions)& functions() const;

	public:
		void push_return(Variable*);
		Variable* pop_return();

	public:
		void push_arguments(const std::vector<Expression*>&);
		std::vector<Expression*> pop_arguments();

	public:
		void hook();
		void unhook();

	public:
		void print(const std::string& message);
		void exit_program(Variable*);

	public:
		Context()
			: _variables{}, _functions{}, _returns{}, _arguments{}
		{
		}

	};
}

#endif
