import std;

#include "InterpreterToolchain.h"
#include "JobManager.h"
#include "InterpreterParser.h"
#include "HazeEvaluator.h"
#include "ErrorReporter.h"

// Haze InterpreterToolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ToolchainType InterpreterToolchain::ttype() const
	{
		return ToolchainType::INTERPRETER;
	}

	void InterpreterToolchain::run(const std::string& filepath)
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "interpreting");

		const auto parse_task = REQUIRE_SAFE(JobManager)->begin_job("parsing");

		ServiceContainer::instance().register_factory<Parser>([&]()
		{
			return std::make_shared<InterpreterParser>(filepath);
		});

		REQUIRE_SAFE(Parser)->reload(_tokens.at(filepath), filepath);

		auto declarators = REQUIRE_SAFE(Parser)->parse();
		REQUIRE_SAFE(JobManager)->end_job(parse_task);

		const auto evaluate_task = REQUIRE_SAFE(JobManager)->begin_job("evaluating");
		const auto evaluator = new HazeEvaluator{ std::move(declarators), filepath };
		
		try
		{
			evaluator->evaluate();
		}

		catch (std::exception)
		{
			USE_SAFE(ErrorReporter)->post_information("exit condition encountered", NULL_TOKEN);
		}

		REQUIRE_SAFE(JobManager)->end_job(evaluate_task);
		USE_SAFE(ErrorReporter)->close_context();
	}
}
