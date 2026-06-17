#ifndef HAZE_BMBUILDER_H
#define HAZE_BMBUILDER_H

#include <builder/Builder.h>

// Haze BMBuilder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct BMBuilder : public Builder
	{
	public:
		virtual void export_exe(const std::filesystem::path&) override;

	public:
		BMBuilder(const ByteRange& executable)
			: Builder{ executable }
		{
		}
	};
}

#endif
