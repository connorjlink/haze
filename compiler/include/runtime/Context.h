#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

#include <ast/AST.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/Evaluation.h>
#include <runtime/Variable.h>
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
	private:
		using VariableMap = std::unordered_map<std::string, VariableHandle>;

	public:
		struct StackFrame
		{
			VariableMap local_variables;
			StackFrame* parent;
		};
		std::vector<StackFrame> stack;
		VariableMap global_variables;

#pragma message("TODO: refactor to allow contiguous memory for stack frames")
	public:
		void enter_function_frame(const std::vector<std::string_view>&);
		void exit_frame();
		void enter_block();
		void exit_block();

	public:
		void print(std::string_view message);
		void exit_program(VariableHandle);

	public:
		VariableHandle resolve(std::string_view);
		void assign(std::string_view, VariableHandle);
	};
}

#endif
