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
		ToolchainKind toolchain_kind() const;
		std::vector<Linkable> run(const std::filesystem::path&);

	public:
		using Toolchain::Toolchain;
	};
}

#endif 
