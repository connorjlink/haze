#ifndef HAZE_BMBUILDER_H
#define HAZE_BMBUILDER_H

#include <builder/Builder.h>

// Haze BMBuilder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class BMBuilder : public Builder
    {
    public:
        BMBuilder(const byterange& executable)
            : Builder{ executable }
        {
        }

    public:
        virtual void export_exe(const std::string&) override;
    };
}

#endif
