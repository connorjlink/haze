import std;

#include "File.h"

// Haze File.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	std::string friendlify_ordinal(int cardinal)
	{
		if (cardinal <= 0)
		{
			return std::to_string(cardinal) + "th";
		}

		// 11th, 12th, and 13th need a special case
		const auto last_two_digits = cardinal % 100;
		if (last_two_digits >= 11 && last_two_digits <= 13)
		{
			return std::to_string(cardinal) + "th";
		}

		const auto last_digit = cardinal % 10;
		switch (last_digit)
		{
			case 1:
				return std::to_string(cardinal) + "st";
			case 2:
				return std::to_string(cardinal) + "nd";
			case 3:
				return std::to_string(cardinal) + "rd";
			default:
				return std::to_string(cardinal) + "th";
		}
	}
}

namespace hz
{
	ToolchainType File::ttype(void) const
	{
		return _type;
	}

	std::string File::raw_contents(void) 
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

			_raw_contents = { source };

			return source;
		}

		USE_SAFE(ErrorReporter).post_uncorrectable(std::format(
			"file {} not found", _filepath), NULL_TOKEN);
	}

	std::string File::processed_contents(void)
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

	void File::reload(void)
	{
		_reload_count++;

		// since the file contents lazy-load and cache as needed, simply deleting the cache 
		// will force a reload upon next access
		_raw_contents.reset();
		_processed_contents.reset();

		USE_SAFE(ErrorReporter).post_information(std::format(
			"reloading file `{}` for the {} time", _filepath, ::friendlify_ordinal(_reload_count)), NULL_TOKEN);
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