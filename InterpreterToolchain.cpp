#include "InterpreterToolchain.h"
#include "JobManager.h"
#include "InterpreterParser.h"
#include "HazeEvaluator.h"
#include "ErrorReporter.h"

namespace hz
{
	ToolchainType InterpreterToolchain::ttype() const
	{
		return ToolchainType::INTERPRETER;
	}

	void InterpreterToolchain::run()
	{
		for (auto& filepath : _filepaths)
		{
			_error_reporter->open_context(filepath, "interpreting");

			const auto parse_task = _job_manager->begin_job("parsing");
			_parser = new InterpreterParser{ _tokens.at(filepath) };
			auto declarators = _parser->parse();
			_job_manager->end_job(parse_task);

			const auto evaluate_task = _job_manager->begin_job("evaluating");
			const auto evaluator = new HazeEvaluator{ std::move(declarators), _context };
			evaluator->evaluate();
			_job_manager->end_job(evaluate_task);

			_error_reporter->close_context();
		}
	}
}