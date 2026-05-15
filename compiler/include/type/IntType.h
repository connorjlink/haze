#ifndef HAZE_INTTYPE_H
#define HAZE_INTTYPE_H

#include <type/Type.h>

// Haze IntType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class IntKind
	{
		CHAR,
		SHORT,
		INT,
		LONG,
		LONG_LONG,
	};

	static const std::unordered_map<IntKind, std::string_view> _int_kind_map
	{
		{ IntKind::CHAR, "char" },
		{ IntKind::SHORT, "short" },
		{ IntKind::INT, "int" },
		{ IntKind::LONG, "long" },
		{ IntKind::LONG_LONG, "long long" },
	};

	class IntType : public TypeBase
	{
	public:
		

	public:
		StorageClass storage;
		TypeQualifier qualifier;
		// C defaults to signed
		TypeSignedness signedness = TypeSignedness::SIGNED;
		IntKind int_kind;

	public:
		TypeKind ttype() const
		{
			return TypeKind::INT;
		}

		Offset size() const
		{
			using enum IntKind;
			switch (int_kind)
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
					CommonErrors::invalid_int_type(int_kind, NULL_TOKEN);
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
