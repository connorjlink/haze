import std;

#include "CompilerToolchain.h"
#include "JobManager.h"
#include "CompilerParser.h"
#include "Generator.h"
#include "CommonToolchain.h"
#include "CompilerLinker.h"
#include "Constants.h"
#include "CommandLineOptions.h"
#include "IntermediateOptimizer.h"
#include "X86Linker.h"
#include "X86Emitter.h"
#include "ErrorContext.h"
#include "ErrorReporter.h"

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
		USE_SAFE(ErrorReporter).open_context(filepath, "compiling");

		const auto parse_task = _job_manager->begin_job("parsing");
		_parser = new CompilerParser{ _tokens.at(filepath), filepath };
		auto ast = _parser->parse();
		_job_manager->end_job(parse_task);


		const auto generate_task = _job_manager->begin_job("generating");
		_generator = new Generator{ ast, filepath };
		auto linkables = _generator->generate();
		_job_manager->end_job(generate_task);


		const auto optimize_task = _job_manager->begin_job("optimizing");
		for (auto& linkable : linkables)
		{
			auto optimizer = new IntermediateOptimizer{ linkable.symbol->name, linkable.ir };
			auto ir_optimized = optimizer->optimize();

			linkable.ir = std::move(ir_optimized);
		}
		_job_manager->end_job(optimize_task);

		// NOTE: old method;
		/*byterange out{};
		for (auto& linkable : linkables)
		{
			for (auto command : linkable.ir)
			{
				out.append_range(command->emit());
			}
		}*/

		const auto link_task = _job_manager->begin_job("linking");
		auto linker = new X86Linker{ linkables };
		auto executable = X86Emitter::emit_init();
		auto out = linker->link();
		executable.append_range(out);
		_job_manager->end_job(link_task);


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

		auto entrypoint = HALF_DWORD_MAX;

		if (_options->_architecture == ArchitectureType::X86)
		{
			//entrypoint = 0x401000 + 0x200;
			//entrypoint = 0x1000;
			entrypoint = 0x41000;
		}

		// shared environment with Assembler/Compiler
		/*auto image = common_link(entrypoint);
		auto executable = common_emit(std::move(image), _filepath);*/

		if (!USE_SAFE(ErrorReporter).had_error())
		{
			common_finalize(executable, filepath);
		}

		USE_SAFE(ErrorReporter).post_information(std::format("wrote fresh executable for `{}`", filepath), NULL_TOKEN);
		USE_SAFE(ErrorReporter).close_context();
	}
}
