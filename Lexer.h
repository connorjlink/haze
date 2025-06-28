#ifndef HAZE_LEXER_H
#define HAZE_LEXER_H

#include "Token.h"
#include "Scanner.h"

// Haze Lexer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// inherits the injected file manager service
	class Lexer : public Scanner
	{
	public:
		void advance(std::size_t = 0);
		void expect(char);
		void expect(const std::string&);
		std::vector<Token> lex();

	public:
		ScannerType stype(void) const noexcept override;

	public:
		Lexer(const std::string& filepath)
			: Scanner{ filepath }
		{
		}
		virtual ~Lexer() = default;
	};
}

#endif
