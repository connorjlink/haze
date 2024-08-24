#ifndef HAZE_TOOLCHAIN_H
#define HAZE_TOOLCHAIN_H

#include "ToolchainType.h"

#include "FileManager.h""

namespace hz
{
	class Toolchain
	{
	protected:
		FileManager _file_manager;

	public:
		virtual ~Toolchain() = default;

	public:
		void panic();

	public:
		virtual ToolchainType ttype() const = 0;
	};
}

#endif
