import std;

#include "Linker.h"

// Haze Linker.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// GLOBALS
	Linker* _linker;
	// GLOBALS

	Linker::Linker(const std::string& filepath)
	{
		USE_SAFE(ErrorReporter).open_context(filepath, "linking");
	}

	Linker::~Linker()
	{
		USE_SAFE(ErrorReporter).close_context();
	}
}
