#ifndef HAZE_FLOATTYPE_H
#define HAZE_FLOATTYPE_H

#include <error/CommonErrors.h>
#include <type/defs/StorageClass.h>
#include <type/defs/TypeQualifier.h>
#include <type/defs/TypeKind.h>
#include <utility/Constants.h>

// Haze FloatType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class FloatKind
	{
#define X(enumerator, name) enumerator,
#include <type/defs/FloatKind.def>
#undef X
	};

	constexpr std::string_view to_string(FloatKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case FloatKind::enumerator: return #name;
#include <type/defs/FloatKind.def>
#undef X
		}

		return "<unknown float kind>";
	}

	class FloatType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		FloatKind float_type;

	public:
		TypeKind ttype() const
		{
			return TypeKind::FLOAT;
		}

		Offset size() const
		{
			using enum FloatKind;
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
