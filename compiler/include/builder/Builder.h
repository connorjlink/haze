#ifndef HAZE_BUILDER_H
#define HAZE_BUILDER_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/Constants.h>

// Haze Builder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class Builder
        : public InjectSingleton<ErrorReporter>
    {
	protected:
		const ByteRange& _code_section;
		ByteRange _binary;
        
    public:
		Builder(const ByteRange& executable)
			: _code_section{ executable }, _binary{}
		{
		}

    public:
        virtual void export_exe(const std::string&) = 0;
    };
}

#endif
