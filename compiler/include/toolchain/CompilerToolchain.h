#ifndef HAZE_COMPILERTOOLCHAIN_H
#define HAZE_COMPILERTOOLCHAIN_H

#include <toolchain/Toolchain.h>

// Haze CompilerToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct CompilerToolchain : public Toolchain<CompilerParser>
	{
	public:
		using Toolchain::Toolchain;

	public:
		ToolchainKind tag_type() const final override;
		std::vector<Linkable> run(std::filesystem::path) final override;
	};
}

#endif 
