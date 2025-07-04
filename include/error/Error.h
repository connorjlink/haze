#ifndef HAZE_ERROR_H
#define HAZE_ERROR_H

#include <error/ErrorType.h>
#include <toolchain/models/Token.h>

// Haze Error.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Error
	{
	private:
		SourceLocation _location;
		std::string _erroring_line;
		std::string _message;
		ErrorType _severity;

	public:
		std::string format() const;

	public:
		Error(SourceLocation location, const std::string& erroring_line, const std::string& message, ErrorType severity)
			: _location{ location }, _erroring_line { erroring_line }, _message{ message }, _severity{ severity }
		{
		}
	};
}

#endif
