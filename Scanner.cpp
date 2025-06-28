import std;

#include "Scanner.h"
#include "FileManager.h"

// Haze Scanner.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Scanner::Scanner(const std::string& filepath)
	{
		_source = _file_manager->get_file(filepath).contents();
	}
}
