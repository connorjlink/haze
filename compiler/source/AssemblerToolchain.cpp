import std;

#include <job/JobManager.h>
#include <toolchain/AssemblerToolchain.h>
#include <toolchain/AssemblerParser.h>
#include <toolchain/CommonToolchain.h>

// Haze AssemblerToolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ToolchainType AssemblerToolchain::ttype() const
	{
		return ToolchainType::ASSEMBLER;
	}

	std::vector<Command*> AssemblerToolchain::run(const std::string& filepath)
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "assembling");

		const auto parse_task = REQUIRE_SAFE(JobManager)->begin_job("parsing");

		REQUIRE_SAFE(Parser)->reload(_tokens.at(filepath), filepath);

		const auto commands = REQUIRE_SAFE(Parser)->parse();
		const auto linkable = Linkable{ commands };

		REQUIRE_SAFE(JobManager)->end_job(parse_task);



		USE_SAFE(ErrorReporter)->close_context();

		return commands;
	}
}