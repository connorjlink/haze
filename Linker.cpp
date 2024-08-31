#include "Linker.h"

// Haze Linker.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Linker::Linker(const std::string& filepath)
	{
		_error_reporter->open_context(filepath, "linking");
	}

	Linker::~Linker()
	{
		_error_reporter->close_context();
	}
}
