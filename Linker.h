#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include "Linkable.h"
#include "Instruction.h"

#include <vector>
#include <string>
#include <utility>
#include <cstdint>

namespace hz
{
	class Linker
	{
	private:
		std::vector<Linkable> linkables;

	public:
		Linker(std::vector<Linkable>&& linkables)
			: linkables{ std::move(linkables) }
		{
		}

	public:
		void optimize();
		std::vector<std::uint8_t> link();
	};
}

#endif
