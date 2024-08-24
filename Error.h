#ifndef HAZE_ERROR_H
#define HAZE_ERROR_H

#include "ErrorType.h"

#include <string>
#include <cstdint>

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
		Error(TokenPosition position, std::string erroring_line, std::string message, ErrorType severity)
			: _position{ position }, _erroring_line { std::move(erroring_line) }, _message{ std::move(message) }, _severity{ severity }
		{
		}
	};
}

#endif
