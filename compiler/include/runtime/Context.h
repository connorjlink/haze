#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

#include <ast/AST.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze Context.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{	
	struct Function;
	struct Expression;
	struct Variable;

	struct Context
		: public SingletonTag<Context>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		std::unordered_map<std::string, Variable*> variables;
		std::vector<Function*> functions;

#pragma message("TODO: refactor the context to not maintain a state--should work on pure functions only!")
		std::stack<Variable*> returns;
		std::stack<std::vector<Expression*>> arguments;

	private:
		bool executing = true;

	public:
		void declare_variable(const std::string&);
		void define_variable(const std::string&, Variable*);
		inline const std::unordered_map<std::string, Variable*>& get_variables() const
		{
			return variables;
		}

		void define_function(Function*);
		inline const std::vector<Function*>& get_functions() const
		{
			return functions;
		}

	public:
		void push_return(Variable*);
		Variable* pop_return();

	public:
		void push_arguments(const std::vector<Expression*>&);
		std::vector<Expression*> pop_arguments();

	public:
		void print(const std::string& message);
		void exit_program(Variable*);

	public:
		Context()
			: variables{}, functions{}, returns{}, arguments{}
		{
		}
	};
}

#endif
