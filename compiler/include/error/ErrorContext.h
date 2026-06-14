#ifndef HAZE_ERRORCONTEXT_H
#define HAZE_ERRORCONTEXT_H

#include <error/models/Error.h>
#include <toolchain/models/Token.h>

// Haze ErrorContext.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct ErrorContext;

	struct ErrorFrame
	{
		ErrorContext* context;
		std::filesystem::path filepath;
	};

	struct ErrorContext
	{
	private:
		std::string_view task;
	
	private:
		std::vector<Error> errors;

	public:
		std::string format() const;

	public:
		std::size_t error_count() const;

	public:
		void post(ErrorKind, const std::string&, const std::string&, const Token&);

	public:
		ErrorContext(const std::string& task)
			: task{ task }
		{
		}
	};
}

#endif
