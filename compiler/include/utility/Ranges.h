#ifndef HAZE_RANGES_H
#define HAZE_RANGES_H

// Haze Ranges.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    template<std::ranges::range R>
    auto operator|(R&& r, auto (*fn)(R&&))
    {
        return fn(std::forward<R>(r));
    }

    auto range_max = [](auto&& r)
    {
        return std::ranges::max(std::forward<decltype(r)>(r));
    };
}

#endif
