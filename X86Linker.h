#ifndef HAZE_X86LINKER_H
#define HAZE_X86LINKER_H

#include "Linkable.h"

// Haze X86Linker.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Linker
	{
	private:
		std::vector<Linkable> _linkables;

	public:
		X86Linker(std::vector<Linkable>&& linkables)
			: _linkables{ std::move(linkables) }
		{
		}

	public:
		byterange link();
	};
}

#endif 
