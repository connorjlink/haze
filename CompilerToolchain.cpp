#include "CompilerToolchain.h"
#include "JobManager.h"
#include "CompilerParser.h"
#include "Generator.h"
#include "CompilerLinker.h"
#include "HazeEmitter.h"
#include "X86Emitter.h"
#include "Simulator.h"
#include "Constants.h"
#include "CommandLineOptions.h"

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
			const auto parse_task = _job_manager->begin_job("parsing");
			_parser = new CompilerParser{ _tokens.at(filepath) };
			auto ast = _parser->parse();
			_job_manager->end_job(parse_task);


			const auto generator_task = _job_manager->begin_job("generating");
			_generator = new Generator{ std::move(ast) };
			auto linkables = _generator->generate();
			_job_manager->end_job(generator_task);


			const auto link_task = _job_manager->begin_job("linking");
			_linker = new CompilerLinker{ std::move(linkables) };
			// begin writing commands at $8000
			auto image = _linker->link(HALF_DWORD_MAX);
			_job_manager->end_job(link_task);


			const auto emit_task = _job_manager->begin_job("emitting");

			switch (_options->_architecture)
			{

			}

#pragma message("TODO: add custom command-line options to support x86 here")
			const auto emitter = new HazeEmitter{ std::move(image) };
			auto executable = emitter->emit();
			_job_manager->end_job(emit_task);


#pragma message("TODO: add custom command-line options to either sim or write to binary dump file")
			Simulator sim{ std::move(executable) };
			sim.reset();
			sim.run();
		}
	}
}
