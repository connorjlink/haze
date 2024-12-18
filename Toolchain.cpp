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
		_filepath = filepath;

		_toolchain_task = _job_manager->begin_job("toolchain execution");

		_error_reporter->open_context(_filepath, "initializing");

		const auto read_task = _job_manager->begin_job("file reading");
		auto& file = _file_manager->get_file(_filepath);
		auto source = file.contents();
		_job_manager->end_job(read_task);


		const auto preprocess_task = _job_manager->begin_job("preprocessing");
		const auto preprocessor = new Preprocessor{ source, _filepath };
		auto source_processed = preprocessor->preprocess();
		_job_manager->end_job(preprocess_task);


		const auto lex_task = _job_manager->begin_job("lexing");
		const auto lexer = new Lexer{ source_processed };
		_tokens[_filepath] = lexer->lex();
		_job_manager->end_job(lex_task);

		_error_reporter->close_context();

		run();

		shut_down(false);
	}

	void hz::Toolchain::shut_down(bool is_panic)
	{
		_job_manager->end_job(_toolchain_task);
		
		if (!is_panic)
		{
			_job_manager->log();
		}

		_error_reporter->close_all_contexts();
		Log::raw(_error_reporter->generate_report());
	}

	void Toolchain::panic()
	{
		shut_down(true);
		std::exit(1);
	}
}