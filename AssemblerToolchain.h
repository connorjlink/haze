#ifndef HAZE_ASSEMBLERTOOLCHAIN_H
#define HAZE_ASSEMBLERTOOLCHAIN_H

#include "Toolchain.h"

namespace hz
{
	class AssemblerToolchain : public Toolchain
	{
	public:
		using Toolchain::Toolchain;

	public:
		virtual ToolchainType ttype() const final override;
		virtual void run() final override;
	};
}

#endif 
