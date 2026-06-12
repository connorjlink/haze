import std;

#include <io/models/File.h>

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
	ToolchainKind File::tag_type(void) const
	{
		return type;
	}

	std::string File::get_raw_contents(void) 
	{
#pragma message("TODO: overhaul with Lazy<T>")

		if (raw_contents.has_value())
		{
			return raw_contents.value();
		}

		if (auto file = std::ifstream(filepath, std::ios::in); 
			file.good())
		{
			const auto filesize = std::filesystem::file_size(filepath);

			std::string source(filesize, '\0');
			source.assign((std::istreambuf_iterator<char>(file)),
						   std::istreambuf_iterator<char>());

			raw_contents = { source };

			return source;
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
			"file {} not found", filepath), NULL_TOKEN);
	}

	std::string File::get_processed_contents(void) const
	{
		if (processed_contents.has_value())
		{
			return processed_contents.value();
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
			"file {} not yet processed", filepath), NULL_TOKEN);
	}

	void File::process(const std::string& contents)
	{
		if (raw_contents.has_value())
		{
			processed_contents = contents;
			return;
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
			"file {} not yet loaded", filepath), NULL_TOKEN);
	}

	void File::reload(void)
	{
		reload_count++;

		// since the file contents lazy-load and cache as needed, simply deleting the cache 
		// will force a reload upon next access
		raw_contents.reset();
		processed_contents.reset();

		USE_SAFE(ErrorReporter)->post_information(std::format(
			"reloading file `{}` for the {} time", filepath, ::friendlify_ordinal(reload_count)), NULL_TOKEN);
	}

	void File::compute_type(void)
	{
		const auto path = std::filesystem::path(filepath);
		const auto extension = path.extension().string();

		if (!_toolchain_map.contains(extension))
		{
			USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
				"unrecognized file extension {}", extension), NULL_TOKEN);
		}

		type = _toolchain_map.at(extension);
	}
}