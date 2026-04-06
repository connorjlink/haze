import std;

#include <builder/BMBuilder.h>

// Haze BMBuilder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    void BMBuilder::export_exe(const std::string& filepath)
    {
        _binary = _code_section;

        auto binfile = std::fstream(filepath, std::ios::binary | std::ios::out);
		binfile.write(reinterpret_cast<const char*>(_binary.data()), _binary.size());
    }
}
