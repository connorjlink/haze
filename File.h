#ifndef HAZE_FILE_H
#define HAZE_FILE_H

#include "ToolchainType.h"
#include "ErrorReporter.h"
#include "DependencyInjector.h"
#include "ErrorReporter.h"

// Haze File.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class File
		: public InjectSingleton<ErrorReporter>
	{
	private:
		std::string _filepath;
		ToolchainType _type;
		std::optional<std::string> _raw_contents = std::nullopt;
		std::optional<std::string> _processed_contents = std::nullopt;

	public:
		const std::string& raw_contents(void);
		const std::string& processed_contents(void);
		// only one toolchain can claim proprietership of a file for now
		ToolchainType ttype(void) const;

	public:
		void compute_type(void);
		void process(const std::string&);

	public:
		File(const std::string& filepath)
			: _filepath{ filepath }, _type{}
		{
		}
	};
}

#endif 
