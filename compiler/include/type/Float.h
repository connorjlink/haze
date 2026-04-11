#ifndef HAZE_FLOAT_H
#define HAZE_FLOAT_H

#include <type/Type.h>
#include <utility/Constants.h>

// Haze Float.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    enum class FloatWidth
    {
        FLOAT,
        DOUBLE,
        LONG_DOUBLE,
    };

    static const std::unordered_map<FloatWidth, std::string_view> _float_width_map
    {
        { FloatWidth::FLOAT, "float" },
        { FloatWidth::DOUBLE, "double" },
        { FloatWidth::LONG_DOUBLE, "long double" },
    };

    class Float : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		FloatWidth float_type;

	public:
		TypeType ttype() const
		{
			return TypeType::FLOAT;
		}

		Offset size() const
		{
			switch (float_type)
			{
				case FloatWidth::FLOAT:
					return sizeof(float);
				case FloatWidth::DOUBLE:
					return sizeof(double);
				case FloatWidth::LONG_DOUBLE:
					return sizeof(long double);
				default:
					CommonErrors::invalid_float_type(float_type, NULL_TOKEN); 
					return -1;
			}
		}
		
        bool is_complete() const
        {
            return true;
        }
	};
}
#endif
