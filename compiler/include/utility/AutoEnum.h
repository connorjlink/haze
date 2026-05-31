#ifndef HAZE_AUTOENUM_H
#define HAZE_AUTOENUM_H

// Haze AutoEnum.h
// (c) Connor J. Link. All Rights Reserved.

#define DEFINE_ENUM_INTERNAL(enummember, switchcase, kinds, type, name, extras) \
	enum class type extras \
	{ \
		kinds(enummember) \
	}; \
	constexpr std::string_view to_string(type kind) \
	{ \
		switch (kind) \
		{ \
			kinds(switchcase) \
		} \
		return "<unknown " #name ">"; \
	}

#define DEFINE_ENUM(enummember, switchcase, kinds, type, name) \
	DEFINE_ENUM_INTERNAL(enummember, switchcase, kinds, type, name, )

#define DEFINE_ENUM_BACKED(enummember, switchcase, kinds, type, name, base) \
	DEFINE_ENUM_INTERNAL(enummember, switchcase, kinds, type, name, base)

#endif
