#include "Emitter.h"
#include "HazeEmitter.h"
#include "X86Emitter.h"
#include "CommandLineOptions.h"
#include "ErrorReporter.h"

// Haze Emitter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// GLOBALS
	Emitter* _emitter;
	// GLOBALS


	Emitter::Emitter(std::vector<InstructionCommand*>&& image, const std::string& filepath)
		: image{ std::move(image) }
	{
		_error_reporter->open_context(filepath, "emitting");
	}

	Emitter::~Emitter()
	{
		_error_reporter->close_context();
	}

	Emitter* Emitter::from_architecture(std::vector<InstructionCommand*>&& image, const std::string& filepath)
	{
		Emitter* emitter = nullptr;

		using enum ArchitectureType;
		switch (_options->_architecture)
		{
			case HAZE: emitter = new HazeEmitter{ std::move(image), filepath }; break;
			case X86: emitter = new X86Emitter{ std::move(image), filepath }; break;
			default: _error_reporter->post_error("invalid architecture type", NULL_TOKEN); break;
		}

		return emitter;
	}
}