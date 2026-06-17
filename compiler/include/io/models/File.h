#ifndef HAZE_FILE_H
#define HAZE_FILE_H

#include <data/DependencyInjector.h>
#include <data/Tracking.h>
#include <error/ErrorReporter.h>
#include <toolchain/defs/ToolchainKind.h>
#include <utility/Lazy.h>

// Haze File.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct File
		: public Trackable
		, public InjectSingleton<ErrorReporter>
	{
	private:
		std::filesystem::path filepath;
		ToolchainKind kind;
		Lazy<std::string> raw_contents;
		std::optional<std::string> processed_contents;
		std::uint32_t reload_count = 0; // Explicitly initialized to 0

	private:
		std::string load_raw_contents() const;

	public:
		std::string get_raw_contents();
		std::string get_processed_contents() const;
		ToolchainKind toolchain_kind() const;

	public:
		void process(std::string);
		void reload();

	public:
		File(const std::filesystem::path& filepath)
			: filepath{ filepath }
			, raw_contents{ &File::load_raw_contents, this }
		{
			const auto extension = filepath.extension().string();

			const auto kind_optional = from_string<ToolchainKind>(extension);
			if (!kind_optional)
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"unrecognized file extension {}", extension), NULL_TOKEN);
				kind = ToolchainKind::COMPILER;
				return;
			}

			kind = *kind_optional;
		}
	};
}

#endif 
