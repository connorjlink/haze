#include "Segment.h"

namespace hz
{
    void Segment::append(const Segment& segment)
    {
        /*
        for (auto byte : segment.code)
        {
            code.emplace_back(byte);
        }
        */

        std::copy(segment.code.begin(), segment.code.end(), code.end());
        source.append(segment.source);
    }
}