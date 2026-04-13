#ifndef HAZE_COMPILERTOOLCHAIN_H
#define HAZE_COMPILERTOOLCHAIN_H

#include <toolchain/Toolchain.h>

// Haze CompilerToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class CompilerToolchain : public Toolchain
	{
	public:
		using Toolchain::Toolchain;

	public:
		virtual ToolchainType ttype() const final override;
		virtual std::vector<Linkable> run(const std::string&) final override;
	};
}

#endif 
