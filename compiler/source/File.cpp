import std;

#include <io/models/File.h>

// Haze File.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	std::string cardinal_to_ordinal(int cardinal)
	{
		if (cardinal <= 0)
		{
			return std::format("{}th", cardinal);
		}

		// 11th, 12th, and 13th need a special case
		const auto last_two_digits = cardinal % 100;
		if (last_two_digits >= 11 && last_two_digits <= 13)
		{
			return std::format("{}th", cardinal);
		}

		const auto last_digit = cardinal % 10;
		switch (last_digit)
		{
			case 1:  return std::format("{}st", cardinal);
			case 2:  return std::format("{}nd", cardinal);
			case 3:  return std::format("{}rd", cardinal);
			default: return std::format("{}th", cardinal);
		}
	}
}

namespace hz
{
	ToolchainKind File::toolchain_kind() const
	{
		return kind;
	}

	std::string File::load_raw_contents() const
	{
		if (auto file = std::ifstream(filepath, std::ios::in); file.good())
		{
			const auto filesize = std::filesystem::file_size(filepath);

			auto source = std::string(filesize, '\0');
			source.assign((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());

			return source;
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
			"file {} not found", filepath), NULL_TOKEN);

		return ""; // Fallback return if post_uncorrectable doesn't terminate
	}

	std::string File::get_raw_contents()
	{
		return raw_contents;
	}

	std::string File::get_processed_contents() const
	{
		if (processed_contents.has_value())
		{
			return processed_contents.value();
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
			"file {} not yet processed", filepath), NULL_TOKEN);

		return "<error>";
	}

	void File::process(std::string contents)
	{
		if (raw_contents.has_value())
		{
			processed_contents = std::move(contents);
			return;
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable(std::format(
			"file {} not yet loaded", filepath), NULL_TOKEN);
	}

	void File::reload()
	{
		reload_count++;

		// Utilize our newly implemented reset mechanics
		raw_contents.reset();
		processed_contents.reset();

		USE_SAFE(ErrorReporter)->post_information(std::format(
			"reloading file `{}` for the {} time", filepath, cardinal_to_ordinal(reload_count)), NULL_TOKEN);
	}
}
