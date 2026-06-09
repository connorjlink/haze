#ifndef HAZE_POINTERTYPE_H
#define HAZE_POINTERTYPE_H

#include <type/defs/TypeKind.h>
#include <utility/Constants.h>

// Haze PointerType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct PointerType : public TypeBase
	{
	public:
		TypeHandle pointee;
		TypeQualifier qualifier;

	public:
		// NOTE: this is static to allow other classes a convenient way to get platform variables
		static Offset size();
		bool is_complete() const;
	};
}

#endif
