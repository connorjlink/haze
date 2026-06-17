#ifndef HAZE_LEXER_H
#define HAZE_LEXER_H

#include <toolchain/Scanner.h>
#include <toolchain/models/Token.h>

// Haze Lexer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// inherits the injected file manager service
	struct Lexer : public Scanner
	{
	public:
		std::vector<Token> lex();

	public:
		ScannerKind scanner_kind() const;

	public:
		Lexer(const std::filesystem::path&);
		Lexer(std::string);
	};
}

#endif
