#ifndef HAZE_INITIALIZER_H
#define HAZE_INITIALIZER_H

#include <cli/ArchitectureType.h>
#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <toolchain/AssemblerParser.h>
#include <toolchain/RISCVAssemblerParser.h>

// Haze Initializer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	inline AssemblerParser* create_assembler_parser(const std::string& filepath)
	{
		const auto architecture = USE_UNSAFE(CommandLineOptions)->architecture;
		switch (architecture)
		{
		case ArchitectureType::RISCV:
			return new RISCVAssemblerParser{ filepath };
		}

		USE_UNSAFE(ErrorReporter)->post_error(
			"unsupported architecture", NULL_TOKEN);
		return nullptr;
	}
}

#endif
