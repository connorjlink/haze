#include "ErrorReporter.h"
#include "Toolchain.h"

namespace hz
{
	void ErrorReporter::validate_context()
	{

	}

	void ErrorReporter::open_context(std::string file, std::string task)
	{
		_open_contexts.emplace(ErrorContext{ file, task });
	}

	void ErrorReporter::close_context()
	{
		const auto context = _open_contexts.top();
		_open_contexts.pop();

		_closed_contexts.emplace_back(context.
	}

	std::string ErrorReporter::generate_report()
	{
		std::string report{};

		for (auto& context : _closed_contexts)
		{
			report += context;
		}

		return report;
	}

	void ErrorReporter::post_information(std::string message, Token token)
	{
		_open_contexts.top().post(ErrorType::INFORMATION, message, token);
	}

	void ErrorReporter::post_warning(std::string message, Token token)
	{
		_open_contexts.top().post(ErrorType::WARNING, message, token);
	}

	void ErrorReporter::post_error(std::string message, Token token)
	{
		_open_contexts.top().post(ErrorType::ERROR, message, token);
	}

	void ErrorReporter::post_uncorrectable(std::string message, Token token)
	{
		_open_contexts.top().post(ErrorType::UNCORRECTABLE, message, token);
		_toolchain.panic();
	}
}