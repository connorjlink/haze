#ifndef HAZE_INTERPRETERTOOLCHAIN_H
#define HAZE_INTERPRETERTOOLCHAIN_H

#include "Toolchain.h"

namespace hz
{
	class InterpreterToolchain : public Toolchain
	{
	public:
		using Toolchain::Toolchain;

	public:
		virtual ToolchainType ttype() const final override;
		virtual void run() final override;
	};
}

#endif
