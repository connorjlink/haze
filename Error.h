#ifndef HAZE_ERROR_H
#define HAZE_ERROR_H

#include "ErrorType.h"

#include <cstdint>

// Haze Error.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct TokenPosition
	{
		std::int32_t _line;
		std::int32_t _column;
	};

	struct Error
	{
	private:
		TokenPosition _position;
		std::string _erroring_line;
		std::string _message;
		ErrorType _severity;

	public:
		std::string format() const;

	public:
		Error(TokenPosition position, const std::string& erroring_line, const std::string& message, ErrorType severity)
			: _position{ position }, _erroring_line { erroring_line }, _message{ message }, _severity{ severity }
		{
		}
	};
}

#endif
