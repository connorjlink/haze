#ifndef HAZE_TYPE_H
#define HAZE_TYPE_H

#include "TypeType.h"
#include "IntTypeType.h"
#include "TypeQualifier.h"
#include "TypeSpecifier.h"
#include "TypeSignedness.h"
#include "TypeStorage.h"
#include "PlatformVariables.h"

// Haze Type.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Type
	{
	public:
		TypeQualifier qualifier;
		TypeStorage storage;

	public:
		Type(TypeQualifier qualifier, TypeStorage storage)
			: qualifier{ qualifier }, storage{ storage }
		{
		}

	public:
		virtual TypeType ttype() const = 0;
	};

	class IntType : public Type
	{
	public:
		TypeSignedness signedness;
		IntTypeType int_type;

	public:
		IntType(TypeQualifier qualifier, TypeSignedness signedness, IntTypeType int_type, TypeStorage storage)
			: Type{ qualifier, storage }, signedness{ signedness }, int_type{ int_type }
		{
		}

	public:
		virtual TypeType ttype() const final override;
	};

	class StructType : public Type
	{
	public:
		std::string tag;

	public:
		StructType(TypeQualifier qualifier, const std::string& tag, TypeStorage storage)
			: Type{ qualifier, storage }, tag{ tag }
		{
		}

	public:
		virtual TypeType ttype() const final override;
	};

	class StringType : public Type
	{
	public:
		platform_address_size address;

	public:
		StringType(TypeQualifier qualifier, TypeStorage storage, platform_address_size address)
			: Type{ qualifier, storage }, address{ address }
		{
		}

	public:
		virtual TypeType ttype() const final override;
	};
}

#endif
