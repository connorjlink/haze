#ifndef HAZE_FUNCTIONTYPE_H
#define HAZE_FUNCTIONTYPE_H

// Haze FunctionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class FunctionType : public TypeBase
    {
    public:
        std::vector<SumHandle> parameters;
        SumHandle return_type;
        bool is_variadic;

    public:
        TypeType ttype() const
        {
            return TypeType::FUNCTION;
        }

        Offset size() const
        {
            return Pointer::size();
        }

        bool is_complete() const
        {
            // this is basically impossible for functions, but it is closer to complete since sizeof() works
            return true;
        }
    };
}

#endif
