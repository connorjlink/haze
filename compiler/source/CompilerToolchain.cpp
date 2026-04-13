import std;

#include <job/JobManager.h>
#include <symbol/Symbol.h>
#include <toolchain/CommonToolchain.h>
#include <toolchain/CompilerToolchain.h>
#include <toolchain/CompilerParser.h>
#include <toolchain/Generator.h>
#include <toolchain/models/Linkable.h>

// Haze CompilerToolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ToolchainType CompilerToolchain::ttype() const
	{
		return ToolchainType::COMPILER;
	}

	std::vector<Linkable> CompilerToolchain::run(const std::string& filepath)
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "compiling");

		const auto parse_task = REQUIRE_SAFE(JobManager)->begin_job("parsing");

		REQUIRE_SAFE(Parser)->reload(_tokens.at(filepath), filepath);
		auto ast = REQUIRE_SAFE(Parser)->parse();

		REQUIRE_SAFE(JobManager)->end_job(parse_task);
		

		const auto generate_task = REQUIRE_SAFE(JobManager)->begin_job("generating");

		REQUIRE_SAFE(Generator)->reload(ast, filepath);
		auto linkables = REQUIRE_SAFE(Generator)->generate();

		REQUIRE_SAFE(JobManager)->end_job(generate_task);

		
		if (USE_UNSAFE(CommandLineOptions)->optimization & TAC)
		{
			const auto optimize_task = REQUIRE_SAFE(JobManager)->begin_job("optimizing");

			for (auto& linkable : linkables)
			{
				linkable.optimize();
			}

			REQUIRE_SAFE(JobManager)->end_job(optimize_task);
		}

		USE_SAFE(ErrorReporter)->close_context();

		return linkables;
	}
}
