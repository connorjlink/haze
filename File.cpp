import std;

#include "File.h"
#include "ErrorReporter.h"

// Haze File.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	ToolchainType File::ttype(void) const
	{
		return _type;
	}

	const std::string& File::raw_contents(void) 
	{
		if (_raw_contents.has_value())
		{
			return _raw_contents.value();
		}

		if (auto file = std::ifstream(_filepath, std::ios::in); 
			file.good())
		{
			const auto filesize = std::filesystem::file_size(_filepath);

			std::string source(filesize, '\0');
			source.assign((std::istreambuf_iterator<char>(file)),
						   std::istreambuf_iterator<char>());

			return source;
		}

		USE_SAFE(ErrorReporter).post_uncorrectable(std::format(
			"file {} not found", _filepath), NULL_TOKEN);
	}

	const std::string& File::processed_contents(void)
	{
		if (_processed_contents.has_value())
		{
			return _processed_contents.value();
		}

		USE_SAFE(ErrorReporter).post_uncorrectable(std::format(
			"file {} not yet processed", _filepath), NULL_TOKEN);
	}

	void File::process(const std::string& contents)
	{
		if (_raw_contents.has_value())
		{
			_processed_contents = contents;
			return;
		}

		USE_SAFE(ErrorReporter).post_uncorrectable(std::format(
			"file {} not yet loaded", _filepath), NULL_TOKEN);
	}

	void File::compute_type(void)
	{
		const auto filepath = std::filesystem::path(_filepath);
		const auto extension = filepath.extension().string();

		if (!_toolchain_map.contains(extension))
		{
			USE_SAFE(ErrorReporter).post_uncorrectable(std::format(
				"unrecognized file extension {}", extension), NULL_TOKEN);
		}

		_type = _toolchain_map.at(extension);
	}
}