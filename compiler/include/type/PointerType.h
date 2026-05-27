#ifndef HAZE_POINTERTYPE_H
#define HAZE_POINTERTYPE_H

#include <type/defs/TypeKind.h>

// Haze PointerType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class PointerType : public TypeBase
	{
	public:
		TypeHandle pointee;
		TypeQualifier qualifier;

	public:
		TypeKind type_kind() const
		{
			return TypeKind::POINTER;
		}

		static Offset size()
		{
			return sizeof(Address);
		}

		bool is_complete() const
		{
			// pointer has a fixed size, so it's always complete to allow for things like void*
			return true;
		}
	};
}

#endif
