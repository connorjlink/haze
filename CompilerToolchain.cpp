#include "CompilerToolchain.h"
#include "JobManager.h"
#include "CompilerParser.h"
#include "Generator.h"
#include "ErrorReporter.h"
#include "CommonToolchain.h"
#include "CompilerLinker.h"
#include "Constants.h"
#include "CommandLineOptions.h"

#include <fstream>
#include <format>

namespace hz
{
	ToolchainType CompilerToolchain::ttype() const
	{
		return ToolchainType::COMPILER;
	}

	void CompilerToolchain::run()
	{
		_error_reporter->open_context(_filepath, "compiling");

		const auto parse_task = _job_manager->begin_job("parsing");
		_parser = new CompilerParser{ _tokens.at(_filepath), _filepath };
		auto ast = _parser->parse();
		_job_manager->end_job(parse_task);


		const auto generate_task = _job_manager->begin_job("generating");
		_generator = new Generator{ std::move(ast), _filepath };
		auto linkables = _generator->generate();
		_job_manager->end_job(generate_task);


		_linker = new CompilerLinker{ std::move(linkables), _filepath };

		auto entrypoint = HALF_DWORD_MAX;

		if (_options->_architecture == ArchitectureType::X86)
		{
			//entrypoint = 0x401000 + 0x200;
			//entrypoint = 0x1000;
			entrypoint = 0x41000;
		}

		// shared environment with Assembler/Compiler
		auto image = common_link(entrypoint);
		auto executable = common_emit(std::move(image), _filepath);

		if (!_error_reporter->_had_error)
		{
			common_finalize(std::move(executable), _filepath);
		}

		_error_reporter->close_context();
	}
}
