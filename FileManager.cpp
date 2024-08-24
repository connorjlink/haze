#include "FileManager.h"
#include "ErrorReporter.h"

#include <format>

namespace hz
{
	void FileManager::open_file(const std::string& filepath)
	{
		auto file = File{ filepath };
		_files[filepath] = file;
	}

	File& FileManager::get_file(const std::string& filepath)
	{
		if (_files.contains(filepath))
		{
			return _files.at(filepath);
		}

		_error_reporter->post_uncorrectable(std::format("file {} was not opened", filepath), NULL_TOKEN);
	}
}