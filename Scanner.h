#ifndef HAZE_SCANNER_H
#define HAZE_SCANNER_H

#include "Token.h"

// Haze Scanner.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ScannerType
	{
		PREPROCESSOR,
		LEXER,
	};

	class Scanner : Injects<FileManager>
	{
	private:
		std::string _source;
		SourceLocation _context;

	public:
		char current(void) const;
		std::size_t where(void) const;
		bool eof(void) const;
		void insert_adjacent(const std::string&);

	public:
		virtual ScannerType stype(void) const noexcept = 0;

	public:
		Scanner(const std::string&);
		virtual ~Scanner() = default;
	};
}

#endif
