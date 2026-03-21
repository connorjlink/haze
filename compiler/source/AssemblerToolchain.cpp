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

	void AssemblerToolchain::run(const std::string& filepath)
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "assembling");

		const auto parse_task = REQUIRE_SAFE(JobManager)->begin_job("parsing");

#error TODO BLOW AWAY THIS SERVICE INTO MAIN() USING EAGER LOAD?
		ServiceContainer::instance().register_factory<Parser, AssemblerParser>([&]()
		{
			return std::make_shared<AssemblerParser>(filepath);
		});

		REQUIRE_SAFE(Parser)->reload(_tokens.at(filepath), filepath);

		auto commands = REQUIRE_SAFE(Parser)->parse();
		REQUIRE_SAFE(JobManager)->end_job(parse_task);

		// NOTE: global singleton linker instance
#error TODO BLOW AWAY THIS SINGLETON INTO MAIN()
		SingletonContainer::instance().register_instance<Linker>
		(
			std::make_shared<Linker>(std::move(commands), filepath)
		);

		// shared environment with Assembler/Compiler
		// 64k region for assembly maximum
		const auto image = common_link(0, UWORD_MAX);
		const auto executable = image
			| std::ranges::views::transform([&](auto&& command) { return command->object_code; })
			| std::ranges::views::join
			| std::ranges::to<std::vector<std::uint8_t>>();

		if (!USE_SAFE(ErrorReporter)->had_error())
		{
			common_finalize(std::move(executable), filepath);
		}

		USE_SAFE(ErrorReporter)->close_context();
	}
}