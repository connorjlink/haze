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
			return self.toolchain_kind_implementation();
		}

		template<typename Self>
		std::vector<Linkable> run(this Self&& self, const std::filesystem::path& filepath)
		{
			return self.run_implementation(filepath);
		}

	public:
		// entry point filepath
		template<typename Self>
		void initialize(this Self&& self, const std::filesystem::path& filepath)
		{
			toolchain_task = REQUIRE_SAFE(JobManager)->begin_job("toolchain execution");

			USE_SAFE(ErrorReporter)->open_context(filepath, "initializing");

			const auto read_task = REQUIRE_SAFE(JobManager)->begin_job("file reading");
			auto& file = USE_SAFE(FileManager)->get_file(filepath);
			const auto& source = file.get_raw_contents();
			REQUIRE_SAFE(JobManager)->end_job(read_task);

			const auto preprocess_task = REQUIRE_SAFE(JobManager)->begin_job("preprocessing");
			auto preprocessor = Preprocessor{ filepath };
			preprocessor.preprocess();
			REQUIRE_SAFE(JobManager)->end_job(preprocess_task);

			const auto lex_task = REQUIRE_SAFE(JobManager)->begin_job("lexing");
			auto lexer = Lexer{ filepath };
			tokens[filepath] = lexer.lex();
			REQUIRE_SAFE(JobManager)->end_job(lex_task);

			USE_SAFE(ErrorReporter)->close_context();

			self.run(filepath);

			shut_down(false);
		}

		void shut_down(bool);
	}; 
}

#endif
