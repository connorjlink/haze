#include "Toolchain.h"
#include "JobManager.h"
#include "FileManager.h"
#include "Preprocessor.h"
#include "Lexer.h"
#include "ErrorReporter.h"
#include "Log.h"


#include <cstdlib>

namespace hz
{
	void Toolchain::init(const std::vector<std::string>& filepaths)
	{
		_filepaths = filepaths;

		_toolchain_task = _job_manager->begin_job("toolchain execution");

		for (auto& filepath : _filepaths)
		{
			_error_reporter->open_context(filepath, "initializing");

			const auto read_task = _job_manager->begin_job("file reading");
			// TODO: figure out where the filepath vector should be
			// currently, this file is opened in main() already
			//_file_manager->open_file(filepath);
			auto& file = _file_manager->get_file(filepath);
			auto source = file.contents();
			_job_manager->end_job(read_task);


			const auto preprocess_task = _job_manager->begin_job("preprocessing");
			const auto preprocessor = new Preprocessor{ std::move(source), filepath };
			auto source_processed = preprocessor->preprocess();
			_job_manager->end_job(preprocess_task);


			const auto lex_task = _job_manager->begin_job("lexing");
			const auto lexer = new Lexer{ std::move(source_processed) };
			_tokens[filepath] = lexer->lex();
			_job_manager->end_job(lex_task);

			_error_reporter->close_context();
		}

		run();

		shut_down();
	}

	void Toolchain::shut_down()
	{
		_job_manager->end_job(_toolchain_task);
		_job_manager->log();

		Log::raw(_error_reporter->generate_report());
	}

	void Toolchain::panic()
	{
		Log::raw(_error_reporter->generate_report());
		std::exit(EXIT_FAILURE);
	}
}