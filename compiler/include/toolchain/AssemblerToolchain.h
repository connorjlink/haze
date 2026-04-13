#ifndef HAZE_ASSEMBLERTOOLCHAIN_H
#define HAZE_ASSEMBLERTOOLCHAIN_H

#include <toolchain/Toolchain.h>

// Haze AssemblerToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AssemblerToolchain : public Toolchain
	{
	public:
		using Toolchain::Toolchain;

	public:
		virtual ToolchainType ttype() const final override;
		virtual std::vector<Linkable> run(const std::string&) final override;
	};
}

#endif 
