#include "File.h"

#include <fstream>
#include <filesystem>

namespace hz
{
	std::string File::contents() const 
	{
		auto file = std::ifstream(_filepath);

		if (!file.good())
		{
			const auto filesize = std::filesystem::file_size(_filepath);

			std::string source(filesize, '\0');
			source.assign((std::istreambuf_iterator<char>(file)),
						   std::istreambuf_iterator<char>());

			return source;
		}

		_error_reporter->post_uncorrectable(std::format("file {} not found", _filepath), NULL_TOKEN);
	}
}