#ifndef HAZE_EXITPROGRAMEXCEPTION_H
#define HAZE_EXITPROGRAMEXCEPTION_H

// Haze ExitProgramException.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ExitProgramException : public std::exception
	{
	private:
		std::string _message;

	public:
		ExitProgramException(const std::string& message)
			: _message{ message }
		{
		}

	public:
		const char* what() const override
		{
			return std::format("Haze Executable - `exit()` invoked (code {})", _message).c_str();
		}
	};
}

#endif 
