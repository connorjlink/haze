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

#include <fstream>
#include <format>

namespace hz
{
	std::vector<InstructionCommand*> common_link()
	{
		const auto link_task = _job_manager->begin_job("linking");
		// begin writing commands at $8000
		auto image = _linker->link(HALF_DWORD_MAX);
		_job_manager->end_job(link_task);
		return image;
	}
	
	std::vector<std::uint8_t> common_emit(std::vector<InstructionCommand*>&& image, const std::string& filepath)
	{
		const auto emit_task = _job_manager->begin_job("emitting");
		Emitter* emitter = nullptr;

		using enum ArchitectureType;
		switch (_options->_architecture)
		{
			case HAZE:
			{
				emitter = new HazeEmitter{ std::move(image), filepath };
			} break;

			case X86:
			{
				emitter = new X86Emitter{ std::move(image), filepath };
			} break;
		}

		auto executable = emitter->emit();
		_job_manager->end_job(emit_task);
		return executable;
	}

	void common_finalize(std::vector<std::uint8_t>&& executable, const std::string& filepath)
	{
		const auto finalize_task = _job_manager->begin_job("finalizing");

		using enum ExecutionType;
		switch (_options->_execution)
		{
			case COMPILE:
			{
				const auto filepath_out = std::format("{}.s", filepath);

				auto outfile = std::ofstream(filepath_out, std::ios::out | std::ios::binary);

				if (!outfile.good())
				{
					_error_reporter->post_uncorrectable(std::format("output file {} not writable", filepath_out), NULL_TOKEN);
					break;
				}

				outfile.write(reinterpret_cast<const char*>(executable.data()), executable.size());
				outfile.close();
			} break;

			case SIMULATE:
			{
				Simulator sim{ std::move(executable) };
				sim.reset();
				sim.run();
			} break;
		}

		_job_manager->end_job(finalize_task);
	}
}