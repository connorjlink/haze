#include "Reducer.h"

namespace hz
{
    Reducer::Reducer(std::vector<std::uint8_t>& bytes)
        : bytes(bytes)
    {
    }

    bool Reducer::reduce_stack()
    {
        for (auto address = 0; address < bytes.size(); address++)
        {

        }
    }

    void Reducer::reduce()
    {
        bool made_changes = false;

        if (true)
        {
            made_changes = reduce_stack();
        }

        if (made_changes)
        {
            reduce();
        }
    }
}