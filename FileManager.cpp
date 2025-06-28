import std;

#include "FileManager.h"
#include "ErrorReporter.h"

// Haze FileManager.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void FileManager::open_file(const std::string& filepath)
	{
		auto file = File{ filepath };
		file.compute_type();
		_files[filepath] = file;
	}

	bool FileManager::has_file(const std::string& filepath)
	{
		return _files.contains(filepath);
	}

	File& FileManager::get_file(const std::string& filepath)
	{
		if (has_file(filepath))
		{
			return _files.at(filepath);
		}

		_error_reporter->post_uncorrectable(std::format("file {} was never opened", filepath), NULL_TOKEN);
	}
}