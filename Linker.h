#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include <array>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

namespace hz
{
	class Linker
	{
	private:
		std::vector<std::pair<std::string, std::vector<std::array<std::uint8_t, 3>>>> code;

	public:
		Linker(std::vector<std::pair<std::string, std::vector<std::array<std::uint8_t, 3>>>>&& code)
			: code(std::move(code))
		{
		}

	public:
		std::vector<std::array<std::uint8_t, 3>> optimize(const std::vector<std::array<std::uint8_t, 3>>&) const;
		std::vector<std::uint8_t> link() const;
	};
}

#endif
