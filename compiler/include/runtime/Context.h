#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

#include <ast/AST.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/Evaluation.h>
#include <runtime/Variable.h>
#include <toolchain/models/Environment.h>
#include <utility/Sum.h>

// Haze Context.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{	
	FORWARD_DECLARE_SUM(Expression)
	FORWARD_DECLARE_SUM(Variable)
	FORWARD_DECLARE_SUM(Function)


#pragma message("TODO: roll this back up into the interpreter class alone")

	struct Context
		: public SingletonTag<Context>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		Environment<VariableHandle> environment;

		void enter_function_frame(const std::vector<std::pair<std::string, VariableHandle>>& arguments)
		{
			environment.enter_function();
			for (const auto& [name, value] : arguments)
			{
				environment.define(name, value);
			}
		}

		void exit_function_frame()
		{
			environment.exit_function();
		}

		void enter_block()
		{
			environment.enter_block();
		}

		void exit_block()
		{
			environment.exit_block();
		}

		std::optional<VariableHandle> find(std::string_view name)
		{
			return environment.find(name);
		}

	public:
		void print(std::string_view message);
		void exit_program(VariableHandle);

	public:
		VariableHandle resolve(std::string_view);
		void assign(std::string_view, VariableHandle);
	};
}

#endif
