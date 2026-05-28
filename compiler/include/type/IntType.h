#ifndef HAZE_INTTYPE_H
#define HAZE_INTTYPE_H

#include <error/CommonErrors.h>
#include <type/defs/StorageClass.h>
#include <type/defs/TypeQualifier.h>
#include <type/defs/TypeSignedness.h>
#include <type/defs/TypeKind.h>

// Haze IntType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class IntKind
	{
#define X(enumerator, name) enumerator,
#include <type/defs/IntKind.x>
#undef X
	};

	constexpr std::string_view to_string(IntKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case IntKind::enumerator: return #name;
#include <type/defs/IntKind.x>
#undef X
		}

		return "<unknown int kind>";
	}

	class IntType : public TypeBase
	{
	public:
		StorageClass storage;
		TypeQualifier qualifier;
		// C defaults to signed
		TypeSignedness signedness = TypeSignedness::SIGNED;
		IntKind int_kind;

	public:
		TypeKind type_knid() const
		{
			return TypeKind::INT;
		}

		Offset size() const
		{
			switch (int_kind)
			{
#define X(enumerator, name) case IntKind::enumerator: return sizeof(name);
#include <type/defs/IntKind.x>
#undef X
			}

			CommonErrors::invalid_int_type(int_kind, NULL_TOKEN);
			return -1;
		}

		bool is_complete() const
		{
			return true;
		}
	};
}

#endif
