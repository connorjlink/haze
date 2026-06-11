#ifndef HAZE_INTERPRETER_H
#define HAZE_INTERPRETER_H

#include <ast/declaration/Declaration.h>
#include <runtime/Context.h>
#include <runtime/Variable.h>

// Haze Interpreter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Interpreter
	{
	private:
		std::vector<DeclarationHandle> declarations;
		Context context;

	public:
		void run()
		{
			for (auto& declaration : declarations)
			{
				auto step = declaration.evaluate(context);
				step.handle.resume();

				if (step.handle.promise().kind == ControlFlowKind::RETURN)
				{
					const auto final_value = step.handle.promise().value;
#pragma message("TODO: clean up frames for this function and bubble up the return value to the caller")
				}
			}
		}

	public:
		Interpreter(std::vector<DeclarationHandle> declarations)
			: declarations{ std::move(declarations) }, context{}
		{
		}

	};
}

#endif
