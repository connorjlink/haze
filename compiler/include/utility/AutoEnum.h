#ifndef HAZE_AUTOENUM_H
#define HAZE_AUTOENUM_H

// Haze AutoEnum.h
// (c) Connor J. Link. All Rights Reserved.

#define DEFINE_ENUM(enummember, switchcase, kinds, type, name) \
	enum class type \
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

#endif
