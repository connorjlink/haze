import std;

#include <builder/PEBuilder.h>
#include <cli/CommandLineOptions.h>
#include <job/JobManager.h>
#include <toolchain/CommonToolchain.h>
#include <toolchain/Emitter.h>
#include <toolchain/Linker.h>
#include <utility/PlatformVariables.h>

// Haze CommonToolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::vector<InstructionCommand*> common_link(native_int entrypoint, native_int size)
	{
		// NOTE: to resolve instruction addresses, the instruction lengths are needed.
		// This is done here by simply emitting the corresponding instruction with dummy values
		// and checking its length. So, make a temporary emitter object here to do so.
		ServiceContainer::instance().register_factory<Emitter>([&]()
		{
			return std::shared_ptr<Emitter>(Emitter::from_architecture({}, ""));
		});

		const auto link_task = REQUIRE_UNSAFE(JobManager)->begin_job("linking");
		// begin writing commands at $8000 for haze
		// and currently 0x401200 for Windows PE 32

		auto image = USE_UNSAFE(Linker)->link(entrypoint, size);
		REQUIRE_UNSAFE(JobManager)->end_job(link_task);
		return image;
	}
	
	std::vector<std::uint8_t> common_emit(std::vector<InstructionCommand*>&& image, const std::string& filepath)
	{
		const auto emit_task = REQUIRE_UNSAFE(JobManager)->begin_job("emitting");

		// NOTE: this is the *actual* machine code emitter
		ServiceContainer::instance().register_factory<Emitter>([&]()
		{
			return std::shared_ptr<Emitter>(Emitter::from_architecture(std::move(image), filepath));
		});

		auto executable = REQUIRE_UNSAFE(Emitter)->emit();
		REQUIRE_UNSAFE(JobManager)->end_job(emit_task);

		return executable;
	}

	void common_finalize(const std::vector<std::uint8_t>& executable, const std::string& filepath)
	{
		const auto finalize_task = REQUIRE_UNSAFE(JobManager)->begin_job("finalizing");

		const auto fullpath = std::filesystem::path(filepath);
		const auto basename = fullpath.stem().string();

		if (USE_UNSAFE(CommandLineOptions)->_raw_output)
		{
			const auto filepath_out = std::format("{}.hzb", basename);

			auto outfile = std::ofstream(filepath_out, std::ios::out | std::ios::binary);

			if (!outfile)
			{
				USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format("output file {} not writable", filepath_out), NULL_TOKEN);
			}

			outfile.write(reinterpret_cast<const char*>(executable.data()), executable.size());
			outfile.close();
		}

		using enum ExecutionType;
		switch (USE_UNSAFE(CommandLineOptions)->_execution)
		{
			case COMPILE:
			{
#pragma message("TODO: for now only one translation unit is supported per executable")
				// later on compiling multiple files into one .exe is desired
				const auto exefile = std::format("{}.exe", basename);

				auto pe_builder = new PEBuilder{ executable };
				pe_builder->export_exe(exefile);
			} break;
		}
		
		REQUIRE_UNSAFE(JobManager)->end_job(finalize_task);
	}
}