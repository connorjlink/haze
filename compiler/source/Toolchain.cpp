import std;

#include <error/ErrorReporter.h>
#include <io/Log.h>
#include <toolchain/Toolchain.h>
#include <toolchain/Preprocessor.h>
#include <toolchain/Lexer.h>

// Haze Toolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Toolchain::initialize(const std::filesystem::path& filepath)
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
		const auto lexer = new Lexer{ filepath };
		tokens[filepath] = lexer->lex();
		REQUIRE_SAFE(JobManager)->end_job(lex_task);

		USE_SAFE(ErrorReporter)->close_context();

		this->run();

		shut_down(false);
	}

	void Toolchain::shut_down(bool is_panic)
	{
		REQUIRE_SAFE(JobManager)->end_job(toolchain_task);
		
		if (!is_panic)
		{
			REQUIRE_SAFE(JobManager)->log();
		}

		USE_SAFE(ErrorReporter)->close_these_contexts();
		Log::raw(USE_SAFE(ErrorReporter)->generate_report());
	}

	void Toolchain::panic()
	{
		shut_down(true);
		std::exit(1);
	}
}