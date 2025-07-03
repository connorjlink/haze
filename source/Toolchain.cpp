import std;

#include "Toolchain.h"
#include "JobManager.h"
#include "FileManager.h"
#include "Preprocessor.h"
#include "Lexer.h"
#include "Log.h"
#include "ErrorReporter.h"

// Haze Toolchain.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Toolchain::init(const std::string& filepath)
	{
		_toolchain_task = REQUIRE_SAFE(JobManager)->begin_job("toolchain execution");

		USE_SAFE(ErrorReporter)->open_context(filepath, "initializing");

		const auto read_task = REQUIRE_SAFE(JobManager)->begin_job("file reading");
		auto& file = USE_SAFE(FileManager)->get_file(filepath);
		const auto& source = file.raw_contents();
		REQUIRE_SAFE(JobManager)->end_job(read_task);


		const auto preprocess_task = REQUIRE_SAFE(JobManager)->begin_job("preprocessing");
		const auto preprocessor = new Preprocessor{ filepath };
		preprocessor->preprocess(filepath);
		REQUIRE_SAFE(JobManager)->end_job(preprocess_task);


		const auto lex_task = REQUIRE_SAFE(JobManager)->begin_job("lexing");
		const auto lexer = new Lexer{ filepath };
		_tokens[filepath] = lexer->lex();
		REQUIRE_SAFE(JobManager)->end_job(lex_task);

		USE_SAFE(ErrorReporter)->close_context();

		run(filepath);

		shut_down(false);
	}

	void Toolchain::shut_down(bool is_panic)
	{
		REQUIRE_SAFE(JobManager)->end_job(_toolchain_task);
		
		if (!is_panic)
		{
			REQUIRE_SAFE(JobManager)->log();
		}

		USE_SAFE(ErrorReporter)->close_all_contexts();
		Log::raw(USE_SAFE(ErrorReporter)->generate_report());
	}

	void Toolchain::panic()
	{
		shut_down(true);
		std::exit(1);
	}
}