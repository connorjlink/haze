#ifndef HAZE_LEXER_H
#define HAZE_LEXER_H

#include <toolchain/Scanner.h>
#include <toolchain/models/Token.h>

// Haze Lexer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// inherits the injected file manager service
	class Lexer : public Scanner
	{
	public:
		std::vector<Token> lex();

	public:
		ScannerType stype(void) const noexcept override;

	public:
		Lexer(const std::string&);
	};
}

#endif
