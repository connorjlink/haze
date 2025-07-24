import std;

#include <cli/CommandLineOptions.h>
#include <toolchain/Emitter.h>
#include <toolchain/X86Emitter.h>

// Haze Emitter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Emitter::Emitter(std::vector<InstructionCommand*>&& image, const std::string& filepath)
		: image{ std::move(image) }
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "emitting");
	}

	Emitter::~Emitter()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}

	Emitter* Emitter::from_architecture(std::vector<InstructionCommand*>&& image, const std::string& filepath)
	{
		Emitter* emitter = nullptr;

		using enum ArchitectureType;
		switch (USE_UNSAFE(CommandLineOptions)->_architecture)
		{
			case X86: emitter = new X86Emitter{ std::move(image), filepath }; break;
			default: USE_UNSAFE(ErrorReporter)->post_error("invalid architecture type", NULL_TOKEN); break;
		}

		return emitter;
	}
}