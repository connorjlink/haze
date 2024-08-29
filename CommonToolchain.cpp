#include "CommonToolchain.h"
#include "JobManager.h"
#include "Linker.h"
#include "CompilerLinker.h"
#include "Constants.h"
#include "Emitter.h"
#include "ArchitectureType.h"
#include "HazeEmitter.h"
#include "X86Emitter.h"
#include "CommandLineOptions.h"
#include "ErrorReporter.h"
#include "Simulator.h"
#include "PEBuilder.h"

#include <fstream>
#include <filesystem>
#include <format>

// Haze CommonToolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::vector<InstructionCommand*> common_link(std::uint32_t entrypoint)
	{
		// NOTE: to resolve instruction addresses, the instruction lengths are needed.
		// This is done here by simply emitting the corresponding instruction with dummy values
		// and checking its length. So, make a temporary emitter object here to do so.
		using enum ArchitectureType;
		switch (_options->_architecture)
		{
			case HAZE: _emitter = new HazeEmitter{ {}, "" }; break;
			case X86: _emitter = new X86Emitter{ {}, "" }; break;
		}

		const auto link_task = _job_manager->begin_job("linking");
		// begin writing commands at $8000 for haze
		// and currently 0x401200 for Windows PE 32

		auto image = _linker->link(entrypoint);
		_job_manager->end_job(link_task);
		return image;
	}
	
	std::vector<std::uint8_t> common_emit(std::vector<InstructionCommand*>&& image, const std::string& filepath)
	{
		const auto emit_task = _job_manager->begin_job("emitting");

		// NOTE: this is the *actual* machine code emitter
		using enum ArchitectureType;
		switch (_options->_architecture)
		{
			case HAZE: _emitter = new HazeEmitter{ std::move(image), filepath }; break;
			case X86: _emitter = new X86Emitter{ std::move(image), filepath }; break;
		}

		auto executable = _emitter->emit();
		_job_manager->end_job(emit_task);
		return executable;
	}

	void common_finalize(std::vector<std::uint8_t>&& executable, const std::string& filepath)
	{
		const auto finalize_task = _job_manager->begin_job("finalizing");

		const auto fullpath = std::filesystem::path(filepath);
		const auto filename = fullpath.stem().string();
		const auto filepath_out = std::format("{}.hzb", filename);

		auto outfile = std::ofstream(filepath_out, std::ios::out | std::ios::binary);

		if (!outfile.good())
		{
			_error_reporter->post_uncorrectable(std::format("output file {} not writable", filepath_out), NULL_TOKEN);
			goto file_error;
		}

		outfile.write(reinterpret_cast<const char*>(executable.data()), executable.size());
		outfile.close();

		using enum ExecutionType;
		switch (_options->_execution)
		{
			case COMPILE:
			{
#pragma message("TODO: for now only one translation unit is supported per executable")
				// later on compiling multiple files into one .exe is desired
				const auto exefile = std::format("{}.exe", filename);

				auto pe_builder = new PEBuilder{ std::move(executable) };
				pe_builder->export_exe(exefile);
			} break;

			case SIMULATE:
			{
				Simulator sim{ std::move(executable) };
				sim.reset();
				sim.run();
			} break;
		}

		file_error:
		_job_manager->end_job(finalize_task);
	}
}