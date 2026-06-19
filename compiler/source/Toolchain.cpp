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