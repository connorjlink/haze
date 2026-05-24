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
#include <type/defs/FloatKind.x>
#undef X
	};

	constexpr std::string_view to_string(FloatKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case FloatKind::enumerator: return #name;
#include <type/defs/FloatKind.x>
#undef X
		}

		return "<unknown float kind>";
	}

	class FloatType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		FloatKind float_kind;

	public:
		TypeKind type_kind() const
		{
			return TypeKind::FLOAT;
		}

		Offset size() const
		{
			switch (float_kind)
			{
#define X(enumerator, name) case FloatKind::enumerator: return sizeof(name);
#include <type/defs/FloatKind.x>
#undef X
			}

			CommonErrors::invalid_float_type(float_kind, NULL_TOKEN);
			return -1;
		}

		bool is_complete() const
		{
			return true;
		}
	};
}

#endif
