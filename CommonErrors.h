#ifndef HAZE_COMMONERRORS_H
#define HAZE_COMMONERRORS_H

// Haze CommonErrors.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ParserType;
	struct Token;

	class CommonErrors
	{
	public:
		static void invalid_parser_type(ParserType, const Token&);

	public:
		static void must_be_lvalue(const std::string&, const Token&);
		static void must_be_rvalue(const std::string&, const Token&);

	public:
		static void must_be_constexpr(const std::string&, const Token&);
	};
}

#endif
