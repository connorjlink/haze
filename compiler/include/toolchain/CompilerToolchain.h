#ifndef HAZE_COMPILERTOOLCHAIN_H
#define HAZE_COMPILERTOOLCHAIN_H

#include <toolchain/Toolchain.h>

// Haze CompilerToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct CompilerToolchain : public Toolchain
	{
	public:
		ToolchainKind toolchain_kind_implementation() const;
		std::vector<Linkable> run_implementation(const std::filesystem::path&);

	public:
		using Toolchain::Toolchain;
	};
}

#endif 
