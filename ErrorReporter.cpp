import std;

#include "ErrorReporter.h"
#include "Toolchain.h"

// Haze ErrorReporter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void ErrorReporter::close_all_contexts()
	{
		while (_active_contexts.size() > 0)
		{
			close_context();
		}
	}

	TestParameters ErrorReporter::open_context(const std::string& file, const std::string& task)
	{
		auto& existing_contexts = _open_contexts[file];
		auto* new_context = &existing_contexts.emplace_back(ErrorContext{ task });

		_active_contexts.emplace(new_context);
		_active_files.emplace(file);

		return { new_context, file };
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
		post_information(_active_contexts.top(), _active_files.top(), message, token);
	}

	void ErrorReporter::post_information(ErrorContext* context, const std::string& file, const std::string& message, Token token)
	{
		if (context != nullptr)
		{
			context->post(ErrorType::INFORMATION, message, file, token);
			return;
		}

		post_information(message, token);
	}


	void ErrorReporter::post_warning(const std::string& message, Token token)
	{
		post_warning(_active_contexts.top(), _active_files.top(), message, token);
	}

	void ErrorReporter::post_warning(ErrorContext* context, const std::string& file, const std::string& message, Token token)
	{
		if (context != nullptr)
		{
			context->post(ErrorType::WARNING, message, file, token);
			return;
		}

		post_warning(message, token);
	}


	void ErrorReporter::post_error(const std::string& message, Token token)
	{
		post_error(_active_contexts.top(), _active_files.top(), message, token);
	}

	void ErrorReporter::post_error(ErrorContext* context, const std::string& file, const std::string& message, Token token)
	{
		_error_count++;
		validate_error_count();

		if (context != nullptr)
		{
			context->post(ErrorType::ERROR, message, file, token);
			return;
		}

		post_error(message, token);
	}


	void ErrorReporter::post_uncorrectable(const std::string& message, Token token)
	{
		post_uncorrectable(_active_contexts.top(), _active_files.top(), message, token);
	}

	void ErrorReporter::post_uncorrectable(ErrorContext* context, const std::string& file, const std::string& message, Token token)
	{
		_error_count++;

		if (context != nullptr)
		{
			context->post(ErrorType::UNCORRECTABLE, message, file, token);
		}

		else
		{
			post_uncorrectable(message, token);
		}
		
		close_all_contexts();
		_toolchain->panic();
	}

}