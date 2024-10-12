#ifndef HAZE_EXITPROGRAMEXCEPTION_H
#define HAZE_EXITPROGRAMEXCEPTION_H

// Haze ExitProgramException.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ExitProgramException : public std::exception
	{
	public:
		const char* what() const override
		{
			return "Haze Executable - `exit()` invoked";
		}
	};
}

#endif 
