#ifndef HAZE_FILE_H
#define HAZE_FILE_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/ToolchainType.h>

// Haze File.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// inherits the injected error reporter singleton
	class File //: public Trackable
		: public InjectSingleton<ErrorReporter>
	{
	private:
		std::string _filepath;
		ToolchainType _type;
		std::optional<std::string> _raw_contents = std::nullopt;
		std::optional<std::string> _processed_contents = std::nullopt;
		int _reload_count = 0;

	public:
		std::string raw_contents(void);
		std::string processed_contents(void);
		// only one toolchain can claim proprietership of a file for now
		ToolchainType ttype(void) const;

	public:
		void compute_type(void);
		void process(const std::string&);
		void reload(void);

	public:
		File(const std::string& filepath)
			//: Trackable{ ENABLE_TRACKING },
			: _filepath{ filepath }, _type{}
		{
		}
	};
}

#endif 
