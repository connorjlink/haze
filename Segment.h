#ifndef HAZE_SEGMENT_H
#define HAZE_SEGMENT_H

#include <cstdint>

#include <vector>
#include <string>

namespace hz
{
    class Segment
    {
    public:
        std::vector<std::uint8_t> code;
        std::string source;

    public:
        void append(const Segment&);
    };
}

#endif //HAZE_SEGMENT_H
