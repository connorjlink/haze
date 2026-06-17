#ifndef HAZE_BUILDER_H
#define HAZE_BUILDER_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/Constants.h>

// Haze Builder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Builder
		: public InjectSingleton<ErrorReporter>
	{
	protected:
		const ByteRange& code_section;
		ByteRange binary;

	public:
		virtual void export_exe(const std::filesystem::path&) = 0;

	public:
		Builder(ByteRange code_section)
			: code_section{ std::move(code_section) }, binary{}
		{
		}
	};
}

#endif
