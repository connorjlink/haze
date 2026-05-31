#ifndef HAZE_COMMANDKIND_H
#define HAZE_COMMANDKIND_H

#include <utility/AutoEnum.h>

// Haze CommandKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define COMMAND_KINDS(X) \
	X(LABEL, LabelCommand, label) \
	X(INSTRUCTION, InstructionCommand, instruction) \
	X(DOTORG, DotorgCommand, .org) \
	X(DOTBYTE, DotbyteCommand, .byte)


#define ENUM_MEMBER(enumerator, type, name) enumerator,
#define SWITCH_CASE(enumerator, type, name) case CommandKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, COMMAND_KINDS, CommandKind, command kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
