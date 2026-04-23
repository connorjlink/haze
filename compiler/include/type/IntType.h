#ifndef HAZE_INTTYPE_H
#define HAZE_INTTYPE_H

#include <type/Type.h>

// Haze IntType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class IntType : public TypeBase
	{
	public:
		enum class Width
		{
			CHAR,
			SHORT,
			INT,
			LONG,
			LONG_LONG,
		};

		static const std::unordered_map<Width, std::string_view> width_map
		{
			{ Width::CHAR, "char" },
			{ Width::SHORT, "short" },
			{ Width::INT, "int" },
			{ Width::LONG, "long" },
			{ Width::LONG_LONG, "long long" },
		};

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
			using enum Width;
			switch (int_type)
			{
				case CHAR:
					return sizeof(char);
				case SHORT:
					return sizeof(short);
				case INT:
					return sizeof(int);
				case LONG:
					return sizeof(long);
				case LONG_LONG:
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
