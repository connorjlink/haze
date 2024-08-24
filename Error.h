#ifndef HAZE_ERROR_H
#define HAZE_ERROR_H

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

	public:
		std::string format() const;

	public:
		Error(TokenPosition position, std::string erroring_line, std::string message)
			: _position{ position }, _erroring_line { std::move(erroring_line) }, _message{ std::move(message) }
		{
		}
	};
}

#endif
