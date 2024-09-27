import std;

#include "CommonErrors.h"
#include "Parser.h"
#include "ErrorReporter.h"

// Haze CommonErrors.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void CommonErrors::invalid_parser_type(ParserType ptype, const Token& token)
	{
		_error_reporter->post_error(std::format("[internal compiler error] invalid parser type `{}`", _parser_type_map.at(ptype)), token);
	}

	void CommonErrors::must_be_lvalue(const std::string& message, const Token& token)
	{
		_error_reporter->post_error(std::format("{} must be a modifiable l-value", message), token);
	}

	void CommonErrors::must_be_rvalue(const std::string& message, const Token& token)
	{
		_error_reporter->post_error(std::format("{} must evaluate to an r-value", message), token);
	}

	void CommonErrors::must_be_constexpr(const std::string& message, const Token& token)
	{
		_error_reporter->post_error(std::format("{} must evaluate to a constant expression", message), token);
	}
}
