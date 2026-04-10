import std;

#include <builder/PEBuilder.h>
#include <cli/CommandLineOptions.h>
#include <job/JobManager.h>
#include <toolchain/CommonToolchain.h>
#include <toolchain/Linker.h>

// Haze CommonToolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::vector<InstructionCommand*> common_link(Address entrypoint, Address size)
	{
		const auto link_task = REQUIRE_UNSAFE(JobManager)->begin_job("linking");
		// begin writing commands at $8000 for haze
		// and currently 0x401200 for Windows PE 32

		auto image = USE_UNSAFE(Linker)->link(entrypoint, size);
		REQUIRE_UNSAFE(JobManager)->end_job(link_task);
		return image;
	}
	
	void common_finalize(const std::vector<std::uint8_t>& executable, const std::string& filepath)
	{
		const auto finalize_task = REQUIRE_UNSAFE(JobManager)->begin_job("finalizing");

		const auto fullpath = std::filesystem::path(filepath);
		const auto basename = fullpath.stem().string();

		if (USE_UNSAFE(CommandLineOptions)->raw_output)
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
		switch (USE_UNSAFE(CommandLineOptions)->execution)
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