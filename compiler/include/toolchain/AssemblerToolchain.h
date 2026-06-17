#ifndef HAZE_ASSEMBLERTOOLCHAIN_H
#define HAZE_ASSEMBLERTOOLCHAIN_H

#include <toolchain/Toolchain.h>

// Haze AssemblerToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct AssemblerToolchain : public Toolchain
	{
	public:
		ToolchainKind toolchain_kind() const;
		std::vector<Linkable> run(const std::filesystem::path&);

	public:
		using Toolchain::Toolchain;
	};
}

#endif 
