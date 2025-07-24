#ifndef HAZE_ERRORCONTEXT_H
#define HAZE_ERRORCONTEXT_H

#include <error/Error.h>
#include <toolchain/models/Token.h>

// Haze ErrorContext.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ErrorContext;

	struct ErrorFrame
	{
		ErrorContext* context;
		std::string filepath;
	};

	class ErrorContext
	{
	private:
		std::string _task;
	
	private:
		std::vector<Error> _errors;

	public:
		std::string format() const;

	public:
		std::size_t error_count() const;

	public:
		void post(ErrorType, const std::string&, const std::string&, const Token&);

	public:
		ErrorContext(const std::string& task)
			: _task{ task }
		{
		}
	};
}

#endif
