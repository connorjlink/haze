#ifndef HAZE_TYPESPECIFIER_H
#define HAZE_TYPESPECIFIER_H

#include "Token.h"

// Haze TypeSpecifier.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    enum class TypeSpecifier
    {
        BYTE,
        NVR,
    };

    static const std::unordered_map<TokenType, TypeSpecifier> _type_specifier_map
    {
        { TokenType::BYTE, TypeSpecifier::BYTE },
        { TokenType::NVR, TypeSpecifier::NVR },
    };
}

#endif 
