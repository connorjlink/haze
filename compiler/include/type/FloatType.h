#ifndef HAZE_FLOATTYPE_H
#define HAZE_FLOATTYPE_H

#include <type/Type.h>
#include <utility/Constants.h>

// Haze FloatType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class Float : public TypeBase
	{
	public:
		enum class Width
		{
			FLOAT,
			DOUBLE,
			LONG_DOUBLE,
		};

		static const std::unordered_map<Width, std::string_view> _float_width_map
		{
			{ Width::FLOAT, "float" },
			{ Width::DOUBLE, "double" },
			{ Width::LONG_DOUBLE, "long double" },
		};

	public:
		StorageClass storage;
		TypeQualifier qualifier;
		Width float_type;

	public:
		TypeType ttype() const
		{
			return TypeType::FLOAT;
		}

		Offset size() const
		{
			using enum Width;
			switch (float_type)
			{
				case FLOAT:
					return sizeof(float);
				case DOUBLE:
					return sizeof(double);
				case LONG_DOUBLE:
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
