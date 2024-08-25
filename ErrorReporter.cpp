#include "ErrorReporter.h"
#include "Toolchain.h"

namespace hz
{
	void ErrorReporter::open_context(const std::string& file, const std::string& task)
	{
		auto& existing_contexts = _open_contexts[file];
		existing_contexts.emplace_back(ErrorContext{ task });

		_active_contexts.emplace(&existing_contexts.back());
		_active_files.emplace(file);
	}

	void ErrorReporter::close_context()
	{
		_closed_contexts.emplace_back(std::tuple{ _active_contexts.top(), _active_files.top() });

		_active_contexts.pop();
		_active_files.pop();
	}

	std::string ErrorReporter::generate_report()
	{
		std::string report{};

		for (auto& [context, filepath]: _closed_contexts)
		{
			if (context->error_count() > 0)
			{
				report += std::format("in {}\n{}", filepath, context->format());
			}
		}

		return report;
	}

	void ErrorReporter::post_information(const std::string& message, Token token)
	{
		_active_contexts.top()->post(ErrorType::INFORMATION, message, _active_files.top(), token);
	}

	void ErrorReporter::post_warning(const std::string& message, Token token)
	{
		_active_contexts.top()->post(ErrorType::WARNING, message, _active_files.top(), token);
	}

	void ErrorReporter::post_error(const std::string& message, Token token)
	{
		_had_error = true;
		_active_contexts.top()->post(ErrorType::ERROR, message, _active_files.top(), token);
	}

	void ErrorReporter::post_uncorrectable(const std::string& message, Token token)
	{
		_had_error = true;
		_active_contexts.top()->post(ErrorType::UNCORRECTABLE, message, _active_files.top(), token);
		close_context();
		_toolchain->panic();
	}
}