#ifndef HAZE_STRUCTORUNIONKIND_H
#define HAZE_STRUCTORUNIONKIND_H

#include <utility/AutoEnum.h>

// Haze StructOrUnionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define STRUCT_OR_UNION_KINDS(X) \
	X(STRUCT, struct) \
	X(UNION,  union)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, StructOrUnionKind)

	DEFINE_ENUM(STRUCT_OR_UNION_KINDS, StructOrUnionKind, struct or union kind)

#undef AUTOENUM_ROUTER
}

#endif
