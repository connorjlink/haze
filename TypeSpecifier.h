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
        WORD,
        DWORD,
        QWORD,
        STRING,
        STRUCT,
        NVR, // "void"
    };

    static const std::unordered_map<TokenType, TypeSpecifier> _type_specifier_token_map
    {
        { TokenType::BYTE, TypeSpecifier::BYTE },
        { TokenType::WORD, TypeSpecifier::WORD },
        { TokenType::DWORD, TypeSpecifier::DWORD },
        { TokenType::QWORD, TypeSpecifier::QWORD },
        { TokenType::STRING, TypeSpecifier::STRING },
        { TokenType::STRUCT, TypeSpecifier::STRUCT },
        { TokenType::NVR, TypeSpecifier::NVR },
    };

    static const std::unordered_map<TypeSpecifier, IntTypeType> _type_specifier_int_map
    {
        { TypeSpecifier::BYTE, IntTypeType::INT8 },
        { TypeSpecifier::WORD, IntTypeType::INT16 },
        { TypeSpecifier::DWORD, IntTypeType::INT32 },
        { TypeSpecifier::QWORD, IntTypeType::INT64 },
    };

    static const std::unordered_map<TypeSpecifier, std::string> _type_specifier_map
    {
        { TypeSpecifier::BYTE, "byte" },
        { TypeSpecifier::WORD, "word" },
        { TypeSpecifier::DWORD, "dword" },
        { TypeSpecifier::QWORD, "qword" },
        { TypeSpecifier::STRING, "string" },
        { TypeSpecifier::STRUCT, "struct" },
        { TypeSpecifier::NVR, "nvr" },
    };
}

#endif 
