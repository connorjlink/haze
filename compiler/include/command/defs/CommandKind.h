#ifndef HAZE_COMMANDKIND_H
#define HAZE_COMMANDKIND_H

#include <utility/AutoEnum.h>

// Haze CommandKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define COMMAND_KINDS(X) \
	X(LABEL,       LabelCommand,       label) \
	X(INSTRUCTION, InstructionCommand, instruction) \
	X(DOTORG,      DotOrgCommand,      .org) \
	X(DOTBYTE,     DotByteCommand,     .byte)


#define ENUM_MEMBER(enumerator, type, name) enumerator,
#define SWITCH_CASE(enumerator, type, name) case CommandKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, type, name) Mapping{ #name, CommandKind::enumerator },
#define FORWARD_DECLARATION(enumerator, type, name) class type;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, COMMAND_KINDS, CommandKind, command kind)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
