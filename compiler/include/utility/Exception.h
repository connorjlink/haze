#ifndef HAZE_EXCEPTION_H
#define HAZE_EXCEPTION_H

// Haze Exception.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct ExitProgramException : public std::exception
	{
	private:
		std::string message;

	public:
		const char* what() const override
		{
			return std::format("Haze Executable - `exit()` invoked (code {})", message).c_str();
		}

	public:
		ExitProgramException(const std::string& message)
			: message{ message }
		{
		}
	};

	struct EvaluationFailureException : public std::exception
	{
	private:
		std::string trace;

	public:
		const char* what() const noexcept
		{
			return trace.c_str();
		}

	public:
		EvaluationFailureException(const std::stacktrace& stacktrace)
			: trace{ to_string(stacktrace) }
		{
		}
	};
}

#endif 
