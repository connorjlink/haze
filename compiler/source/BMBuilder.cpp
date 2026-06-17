import std;

#include <builder/BMBuilder.h>

// Haze BMBuilder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void BMBuilder::export_exe(const std::filesystem::path& filepath)
	{
		binary = code_section;

		auto binfile = std::fstream{ filepath, std::ios::binary | std::ios::out };
		binfile.write(reinterpret_cast<const char*>(binary.data()), binary.size());
	}
}
