#ifndef HAZE_STATEMENTKIND_H
#define HAZE_STATEMENTKIND_H

#include <utility/AutoEnum.h>

// Haze StatementKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#include <ast/statement/defs/StatementKind.x>

#define ENUM_MEMBER(enumerator, type, name) enumerator,
#define SWITCH_CASE(enumerator, type, name) case StatementKind::type: return #name;
	
	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, STATEMENT_KINDS, StatementKind, statement kind)
}

#endif