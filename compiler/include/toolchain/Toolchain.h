#ifndef HAZE_TOOLCHAIN_H
#define HAZE_TOOLCHAIN_H

#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <io/FileManager.h>
#include <job/JobManager.h>
#include <toolchain/defs/ToolchainKind.h>
#include <toolchain/Generator.h>
#include <toolchain/Parser.h>
#include <toolchain/models/Token.h>
#include <toolchain/Linker.h>

// Haze Toolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Toolchain
		: public ServiceTag<Toolchain>
		, public InjectService<Generator, JobManager, Parser>
		, public InjectSingleton<CommandLineOptions, ErrorReporter, FileManager, Linker>
	{
	protected:
		std::unordered_map<std::filesystem::path, std::vector<Token>> tokens;

	private:
		std::int32_t toolchain_task = -1;

	public:
		void panic();

	public:
		template<typename Self>
		ToolchainKind toolchain_kind(this Self&& self)
		{
			return std::forward<Self>(self).toolchain_kind_implementation();
		}

		template<typename Self>
		std::vector<Linkable> run(const std::filesystem::path& filepath)
		{
			return self.run_implementation(filepath);
		}

	public:
		// entry point filepath
		void initialize(const std::filesystem::path&);
		// is_panic
		void shut_down(bool);
	}; 
}

#endif
