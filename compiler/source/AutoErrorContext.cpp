import std;

#include <error/AutoErrorContext.h>
#include <toolchain/models/Token.h>

// Haze AutoErrorContext.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	AutoErrorContext::AutoErrorContext(const std::string& file, const std::string& task)
	{
		_initial_context_count = USE_SAFE(ErrorReporter)->get_context_count();
		_frame = USE_SAFE(ErrorReporter)->open_context(file, task);
	}

	AutoErrorContext::~AutoErrorContext()
	{
		auto current_context_count = USE_SAFE(ErrorReporter)->get_context_count();

		if (current_context_count != _initial_context_count)
		{
			USE_SAFE(ErrorReporter)->post_warning(_frame.context, _file, "encountered a mismatched error context; attempting to correct", NULL_TOKEN);

			// close until we have matched
			while (current_context_count = USE_SAFE(ErrorReporter)->get_context_count(),
				current_context_count >= _initial_context_count)
			{
				USE_SAFE(ErrorReporter)->close_context();
			}

			return;
		}

		USE_SAFE(ErrorReporter)->close_context();
	}
}
