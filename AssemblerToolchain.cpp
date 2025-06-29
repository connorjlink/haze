import std;

#include "AssemblerToolchain.h"
#include "JobManager.h"
#include "AssemblerParser.h"
#include "AssemblerLinker.h"
#include "Constants.h"
#include "HazeEmitter.h"
#include "X86Emitter.h"
#include "CommonToolchain.h"
#include "ErrorReporter.h"

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
		USE_SAFE(ErrorReporter).open_context(filepath, "assembling");

		const auto parse_task = _job_manager->begin_job("parsing");
		_parser = new AssemblerParser{ _tokens.at(filepath), filepath };
		auto commands = _parser->parse();
		_job_manager->end_job(parse_task);


		_linker = new AssemblerLinker{ std::move(commands), AS_ASSEMBLER_PARSER(_parser), filepath };
		// shared environment with Assembler/Compiler
		auto image = common_link();
		auto executable = common_emit(std::move(image), filepath);

		if (!USE_SAFE(ErrorReporter).had_error())
		{
			common_finalize(std::move(executable), filepath);
		}

		USE_SAFE(ErrorReporter).close_context();
	}
}