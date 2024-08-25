#include "CompilerToolchain.h"
#include "JobManager.h"
#include "CompilerParser.h"
#include "Generator.h"
#include "ErrorReporter.h"
#include "CommonToolchain.h"
#include "CompilerLinker.h"

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
		for (auto& filepath : _filepaths)
		{
			_error_reporter->open_context(filepath, "compiling");

			const auto parse_task = _job_manager->begin_job("parsing");
			_parser = new CompilerParser{ _tokens.at(filepath) };
			auto ast = _parser->parse();
			_job_manager->end_job(parse_task);


			const auto generator_task = _job_manager->begin_job("generating");
			_generator = new Generator{ std::move(ast) };
			auto linkables = _generator->generate();
			_job_manager->end_job(generator_task);


			_linker = new CompilerLinker{ std::move(linkables) };
			// shared environment with Assembler/Compiler
			auto image = common_link();
			auto executable = common_emit(std::move(image));
			common_finalize(std::move(executable), filepath);
			
			_error_reporter->close_context();
		}
	}
}
