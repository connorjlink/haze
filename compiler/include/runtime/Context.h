#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

#include <ast/AST.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/Sum.h>

// Haze Context.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{	
	FORWARD_DECLARE_SUM(Expression)
	FORWARD_DECLARE_SUM(Variable)
	FORWARD_DECLARE_SUM(Function)


	struct Context
		: public SingletonTag<Context>
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using VariableMap = std::unordered_map<std::string, VariableHandle>;

#pragma message("TODO: refactor to allow contiguous memory for stack frames")
		struct StackFrame
		{
			VariableMap local_variables;
			StackFrame* parent;
		};

		std::vector<StackFrame> stack;
		VariableMap global_variables;

	public:
		void enter_function_frame(const std::vector<std::string_view>&);
		void exit_frame();
		void enter_block();
		void exit_block();

		VariableHandle resolve(std::string_view);
		void assign(std::string_view name, VariableHandle value);



	private:
		std::unordered_map<std::string, VariableHandle> variables;
		std::vector<FunctionHandle> functions;

#pragma message("TODO: refactor the context to not maintain a state--should work on pure functions only!")
		std::stack<VariableHandle> returns;
		std::stack<std::vector<ExpressionHandle>> arguments;

	private:
		bool executing = true;

	public:
		void declare_variable(const std::string&);
		void define_variable(const std::string&, VariableHandle);
		inline const std::unordered_map<std::string, VariableHandle>& get_variables() const
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
