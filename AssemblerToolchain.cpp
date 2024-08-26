#include "AssemblerToolchain.h"
#include "JobManager.h"
#include "AssemblerParser.h"
#include "AssemblerLinker.h"
#include "Constants.h"
#include "HazeEmitter.h"
#include "X86Emitter.h"
#include "CommonToolchain.h"
#include "ErrorReporter.h"

namespace hz
{
	ToolchainType AssemblerToolchain::ttype() const
	{
		return ToolchainType::ASSEMBLER;
	}

	void AssemblerToolchain::run()
	{
		_error_reporter->open_context(_filepath, "assembling");

		const auto parse_task = _job_manager->begin_job("parsing");
		_parser = new AssemblerParser{ _tokens.at(_filepath), _filepath };
		auto commands = _parser->parse();
		_job_manager->end_job(parse_task);


		_linker = new AssemblerLinker{ std::move(commands), AS_ASSEMBLER_PARSER(_parser), _filepath };
		// shared environment with Assembler/Compiler
		auto image = common_link();
		auto executable = common_emit(std::move(image), _filepath);

		if (!_error_reporter->_had_error)
		{
			common_finalize(std::move(executable), _filepath);
		}

		_error_reporter->close_context();
	}
}