#ifndef HAZE_VARIANT_H
#define HAZE_VARIANT_H

// Haze Variant.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<template<typename> typename TraitT, typename... Ts>
		requires (TraitT<Ts>::value && ...)
	using ConstrainedVariant = std::variant<Ts...>;
}

#endif
