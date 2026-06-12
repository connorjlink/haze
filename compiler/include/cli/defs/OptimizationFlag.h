#ifndef HAZE_OPTIMIZATIONFLAG_H
#define HAZE_OPTIMIZATIONFLAG_H

#include <utility/AutoBitfield.h>

// Haze OptimizationFlag.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define OPTIMIZATION_FLAGS(X) \
	X(NONE, none,      0) \
	X(AST,  ast,  1 << 0) \
	X(TAC,  tac,  1 << 1) \
	X(LTO,  lto,  1 << 2)


#define AUTOBITFIELD_ROUTER(X, enumerator, name, value) X(enumerator, name, value, OptimizationFlag)

	DEFINE_BITFIELD_BASE(OPTIMIZATION_FLAGS, OptimizationFlag, optimization flag, std::uint8_t)

#undef AUTOBITFIELD_ROUTER
}

#endif 
