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


#define AUTOENUM_ROUTER(X, enumerator, type, name) X(enumerator, type, name, CommandKind)

	DEFINE_ENUM(COMMAND_KINDS, CommandKind, command kind)

#undef AUTOENUM_ROUTER
}

#endif
