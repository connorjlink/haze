import std;

#include <job/JobManager.h>
#include <toolchain/AssemblerToolchain.h>
#include <toolchain/AssemblerParser.h>

// Haze AssemblerToolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ToolchainKind AssemblerToolchain::toolchain_kind() const
	{
		return ToolchainKind::ASSEMBLER;
	}

	std::vector<Linkable> AssemblerToolchain::run(const std::filesystem::path& filepath)
	{
		const auto _ = USE_SAFE(ErrorReporter)->auto

		USE_SAFE(ErrorReporter)->open_context(filepath, "assembling");

		const auto parse_task = REQUIRE_SAFE(JobManager)->begin_job("parsing");

		REQUIRE_SAFE(Parser)->reload(tokens.at(filepath), filepath);

		const auto commands = REQUIRE_SAFE(Parser)->parse();
		const auto linkable = Linkable{ commands };

		REQUIRE_SAFE(JobManager)->end_job(parse_task);

		USE_SAFE(ErrorReporter)->close_context();

		return linkable;
	}
}