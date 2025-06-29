import std;

#include "FileManager.h"

// Haze FileManager.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void FileManager::open_file(const std::string& filepath)
	{
		if (has_file(filepath))
		{
			auto& file = _files.at(filepath);
			file.reload();
		}
		
		auto file = File{ filepath };
		file.compute_type();
		_files.emplace(filepath, file);
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

		never_opened_error(filepath);
	}

	void FileManager::update_file(const std::string& filepath, const std::string& content)
	{
		if (has_file(filepath))
		{
			auto& file = _files.at(filepath);
			file.process(content);
			return;
		}

		never_opened_error(filepath);
	}
	
	void FileManager::never_opened_error(const std::string& filepath) const
	{
		USE_UNSAFE(ErrorReporter).post_uncorrectable(std::format(
			"file {} was never opened", filepath), NULL_TOKEN);
	}
}