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
	struct FunctionArgument
	{
		std::string_view name;
		VariableHandle value;
	};

	struct Context 
		: public Environment<VariableHandle>
		, public SingletonTag<Context>
	{
	public:
		void enter_function(const std::vector<FunctionArgument>&);
		void print(std::string_view message);
		void exit_program(VariableHandle);
	};
}

#endif
