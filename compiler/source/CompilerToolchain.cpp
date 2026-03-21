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

	void CompilerToolchain::run(const std::string& filepath)
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "compiling");

		ServiceContainer::instance().register_factory<Parser>([&]
		{
			return std::make_shared<CompilerParser>(filepath);
		});


		const auto parse_task = REQUIRE_SAFE(JobManager)->begin_job("parsing");

		REQUIRE_SAFE(Parser)->reload(_tokens.at(filepath), filepath);
		auto ast = REQUIRE_SAFE(Parser)->parse();

		REQUIRE_SAFE(JobManager)->end_job(parse_task);
		

		const auto generate_task = REQUIRE_SAFE(JobManager)->begin_job("generating");

		REQUIRE_SAFE(Generator)->reload(ast, filepath);
		auto linkables = REQUIRE_SAFE(Generator)->generate();

		REQUIRE_SAFE(JobManager)->end_job(generate_task);


		const auto optimize_task = REQUIRE_SAFE(JobManager)->begin_job("optimizing");
		for (auto& linkable : linkables)
		{
			linkable.optimize();
		}
		REQUIRE_SAFE(JobManager)->end_job(optimize_task);


		const auto link_task = REQUIRE_SAFE(JobManager)->begin_job("linking");

		const auto origin = REQUIRE_SAFE(Generator)->resolve_origin();
		const auto commands = USE_SAFE(Linker)->link(origin, UDWORD_MAX);

		const auto executable = commands
			| std::ranges::views::transform([](auto command) { return command->object_code; })
			| std::ranges::views::join
			| std::ranges::to<std::vector<std::uint8_t>>();

		REQUIRE_SAFE(JobManager)->end_job(link_task);


		// NOTE: the following snippet will format the machine code as readable hex; useful for debugging!
		/*for (auto byte : out)
		{
			std::print("{:02X} ", byte);
		}
		std::println();*/


		/*if (_error_reporter->had_error())
		{
			common_finalize(std::move(executable), _filepath);
			return;
		}*/

		//_linker = new CompilerLinker{ std::move(linkables), _filepath };

		auto entrypoint = 0;

		if (USE_SAFE(CommandLineOptions)->_architecture == ArchitectureType::X86)
		{
			//entrypoint = 0x401000 + 0x200;
			//entrypoint = 0x1000;
			entrypoint = 0x41000;
		}

		if (!USE_SAFE(ErrorReporter)->had_error())
		{
			common_finalize(executable, filepath);
		}

		USE_SAFE(ErrorReporter)->post_information(std::format("wrote fresh executable for `{}`", filepath), NULL_TOKEN);
		USE_SAFE(ErrorReporter)->close_context();
	}
}
