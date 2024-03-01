#ifndef HAZE_REDUCER_H
#define HAZE_REDUCER_H

#include <cstdint>
#include <vector>

namespace hz
{
    class Reducer
    {
    private:
        std::vector<std::uint8_t> bytes;

    private:
        bool reduce_stack();

    public:
        void reduce();

    public:
        std::vector<std::uint8_t>& get_bytes()
        {
            return bytes;
        }

    public:
        explicit Reducer(std::vector<std::uint8_t>&);
    };
}


#endif //HAZE_REDUCER_H
