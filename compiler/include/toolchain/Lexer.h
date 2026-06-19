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
	private:
		// unchecked literal suffixes or empty if no suffix
		std::string_view read_integer_literal_suffix();
		std::string_view read_float_literal_suffix();

	private:
		Token expect_identifier();
		Token expect_integer_literal();
		Token expect_float_literal();
		Token expect_string_literal();
		Token expect_wide_string_literal();
		Token expect_character_literal();

	public:
		ScannerKind scanner_kind() const;
		std::vector<Token> lex();

	public:
		Lexer(const std::filesystem::path&);
		Lexer(std::string);
	};
}

#endif
