import std;

#include <io/FileManager.h>

// Haze FileManager.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void FileManager::open_file(const std::filesystem::path& filepath)
	{
		if (has_file(filepath))
		{
			auto& file = files.at(filepath);
			file.reload();
		}
		
		auto file = File{ filepath };
		file.compute_kind();
		files.emplace(filepath, file);
	}

	bool FileManager::has_file(const std::filesystem::path& filepath)
	{
		return files.contains(filepath);
	}

	File& FileManager::get_file(const std::filesystem::path& filepath)
	{
		if (has_file(filepath))
		{
			return files.at(filepath);
		}

		never_opened_error(filepath);
	}

	void FileManager::update_file(const std::filesystem::path& filepath, std::string content)
	{
		if (has_file(filepath))
		{
			auto& file = files.at(filepath);
			file.process(std::move(content));
			return;
		}

		never_opened_error(filepath);
	}
	
	void FileManager::never_opened_error(const std::filesystem::path& filepath) const
	{
		USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
			"file {} was never opened", filepath), NULL_TOKEN);
	}
}