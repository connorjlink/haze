#ifndef HAZE_FILE_H
#define HAZE_FILE_H

#include <data/DependencyInjector.h>
#include <data/Tracking.h>
#include <error/ErrorReporter.h>
#include <toolchain/defs/ToolchainKind.h>

// Haze File.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// inherits the injected error reporter singleton
	struct File 
		: public Trackable
		, public InjectSingleton<ErrorReporter>
	{
	private:
		std::filesystem::path filepath;
		ToolchainKind type;
		std::optional<std::string> raw_contents = std::nullopt;
		std::optional<std::string> processed_contents = std::nullopt;
		std::uint32_t reload_count = 0;

	public:
		std::string get_raw_contents();
		std::string get_processed_contents() const;
		// only one toolchain can claim proprietership of a file for now
		ToolchainKind toolchain_kind() const;

	public:
		void compute_type();
		void process(const std::string&);
		void reload();

	public:
		File(const std::string& filepath)
			//: Trackable{ ENABLE_TRACKING },
			: filepath{ filepath }, type{}
		{
		}
	};
}

#endif 
