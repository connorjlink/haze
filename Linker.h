#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include "Utility.h"

#include <vector>
#include <string>
#include <utility>
#include <cstdint>

namespace hz
{
	class Linker
	{
	private:
		std::vector<Linkable> object_code;

	public:
		Linker(std::vector<Linkable>&& object_code)
			: object_code(std::move(object_code))
		{
		}

	public:
		std::vector<Instruction*> optimize(const std::vector<Instruction*>&) const;
		std::vector<std::uint8_t> link() const;
	};
}

#endif
