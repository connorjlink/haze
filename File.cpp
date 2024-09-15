#include "File.h"
#include "ErrorReporter.h"

import std;

// Haze File.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ToolchainType File::ttype() const
	{
		return _type;
	}

	std::string File::contents() 
	{
		if (auto file = std::ifstream(_filepath, std::ios::in); 
			file.good())
		{
			const auto filesize = std::filesystem::file_size(_filepath);

			std::string source(filesize, '\0');
			source.assign((std::istreambuf_iterator<char>(file)),
						   std::istreambuf_iterator<char>());

			return source;
		}

		_error_reporter->post_uncorrectable(std::format("file {} not found", _filepath), NULL_TOKEN);
	}

	void File::compute_type()
	{
		const auto filepath = std::filesystem::path(_filepath);
		const auto extension = filepath.extension().string();

		if (!_toolchain_map.contains(extension))
		{
			_error_reporter->post_uncorrectable(std::format("unrecognized file extension {}", extension), NULL_TOKEN);
		}

		_type = _toolchain_map.at(extension);
	}
}