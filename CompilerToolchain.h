#ifndef HAZE_COMPILERTOOLCHAIN_H
#define HAZE_COMPILERTOOLCHAIN_H

#include "Toolchain.h"

// Haze CompilerToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class CompilerToolchain : public Toolchain
	{
	public:
		using Toolchain::Toolchain;

	public:
		virtual ~CompilerToolchain() = default;

	public:
		virtual ToolchainType ttype() const final override;
		virtual void run(const std::string&) final override;
	};
}

#endif 
