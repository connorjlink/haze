#include "Emitter.h"
#include "ErrorReporter.h"

// Haze Emitter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Emitter::Emitter(std::vector<InstructionCommand*>&& image, const std::string& filepath)
		: image{ std::move(image) }
	{
		_error_reporter->open_context(filepath, "emitting");
	}

	Emitter::~Emitter()
	{
		_error_reporter->close_context();
	}
}