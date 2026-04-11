#ifndef HAZE_INT_H
#define HAZE_INT_H

#include <type/Type.h>

// Haze Int.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    enum class IntWidth
	{
		CHAR,
		SHORT,
		INT,
		LONG,
		LONG_LONG,
	};

	static const std::unordered_map<IntWidth, std::string_view> _int_width_map
	{
		{ IntWidth::CHAR, "char" },
		{ IntWidth::SHORT, "short" },
		{ IntWidth::INT, "int" },
		{ IntWidth::LONG, "long" },
		{ IntWidth::LONG_LONG, "long long" },
	};

    class Int : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		// C defaults to signed
		TypeSignedness signedness = TypeSignedness::SIGNED;
		IntWidth int_type;

	public:
		TypeType ttype() const
		{
			return TypeType::INT;
		}

		Offset size() const
		{
			switch (int_type)
			{
				case IntWidth::CHAR:
					return sizeof(char);
				case IntWidth::SHORT:
					return sizeof(short);
				case IntWidth::INT:
					return sizeof(int);
				case IntWidth::LONG:
					return sizeof(long);
				case IntWidth::LONG_LONG:
					return sizeof(long long);
				default:
					CommonErrors::invalid_int_type(int_type, NULL_TOKEN); 
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
