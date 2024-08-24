#ifndef HAZE_ERRORCONTEXT_H
#define HAZE_ERRORCONTEXT_H

#include <string>
#include <vector>
#include <cstdlib>

#include "Error.h"
#include "ErrorType.h"
#include "Token.h"

namespace hz
{
	class ErrorContext
	{
	private:
		std::string _file;
		std::string _task;
	
	private:
		std::vector<Error> _errors;

	public:
		std::string format() const;

	public:
		std::size_t error_count() const;

	public:
		void post(ErrorType, std::string, Token);

	public:
		ErrorContext(std::string file, std::string task)
			: _file{ std::move(file) }, _task{ std::move(task) }
		{
		}
	};
}

#endif
