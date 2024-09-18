#include "InterpreterToolchain.h"
#include "JobManager.h"
#include "InterpreterParser.h"
#include "HazeEvaluator.h"
#include "ErrorReporter.h"

import std;

// Haze InterpreterToolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ToolchainType InterpreterToolchain::ttype() const
	{
		return ToolchainType::INTERPRETER;
	}

	void InterpreterToolchain::run()
	{
		_error_reporter->open_context(_filepath, "interpreting");

		const auto parse_task = _job_manager->begin_job("parsing");
		_parser = new InterpreterParser{ _tokens.at(_filepath), _filepath };
		auto declarators = _parser->parse();
		_job_manager->end_job(parse_task);

		const auto evaluate_task = _job_manager->begin_job("evaluating");
		const auto evaluator = new HazeEvaluator{ std::move(declarators), _filepath };
		
		try
		{
			evaluator->evaluate();
		}

		catch (std::exception)
		{
			_error_reporter->post_information("exit condition encountered", NULL_TOKEN);

		}

		_job_manager->end_job(evaluate_task);

		_error_reporter->close_context();
	}
}