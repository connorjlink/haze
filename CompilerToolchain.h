#ifndef HAZE_COMPILERTOOLCHAIN_H
#define HAZE_COMPILERTOOLCHAIN_H

#include "Toolchain.h"

namespace hz
{
	class CompilerToolchain : public Toolchain
	{
	public:
		using Toolchain::Toolchain;

	public:
		virtual ToolchainType ttype() const final override;
		virtual void run() final override;
	};
}

#endif 
