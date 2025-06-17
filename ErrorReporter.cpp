import std;

#include "ErrorReporter.h"
#include "Toolchain.h"

// Haze ErrorReporter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void ErrorReporter::close_all_contexts()
	{
		while (_active_frames.size() > 0)
		{
			close_context();
		}
	}

	std::size_t ErrorReporter::get_context_count(void)
	{
		return _active_frames.size();
	}

	TestParameters ErrorReporter::open_context(const std::string& file, const std::string& task)
	{
		auto& existing_contexts = _open_frames[file];
		auto* new_context = &existing_contexts.emplace_back(ErrorContext{ task });

		const ErrorFrame frame{ file, new_context };
		_active_frames.emplace(frame);

		return { new_context, file };
	}

	void ErrorReporter::close_context()
	{
		_closed_frames.emplace_back(_active_frames.top());
		_active_frames.pop();
	}

	std::string ErrorReporter::generate_report()
	{
		std::string report{};

		for (auto& frame : _closed_frames)
		{
			if (frame.context->error_count() > 0)
			{
				report += std::format("in {}\n{}", frame.filepath, frame.context->format());
			}
		}

		return report;
	}


	void ErrorReporter::post_information(const std::string& message, const Token& token)
	{
		const auto& frame = _active_frames.top();
		post_information(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_information(ErrorContext* context, const std::string& file, const std::string& message, const Token& token)
	{
		if (context != nullptr)
		{
			context->post(ErrorType::INFORMATION, message, file, token);
			return;
		}

		post_information(message, token);
	}


	void ErrorReporter::post_warning(const std::string& message, const Token& token)
	{
		const auto& frame = _active_frames.top();
		post_warning(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_warning(ErrorContext* context, const std::string& file, const std::string& message, const Token& token)
	{
		if (context != nullptr)
		{
			context->post(ErrorType::WARNING, message, file, token);
			return;
		}

		post_warning(message, token);
	}


	void ErrorReporter::post_error(const std::string& message, const Token& token)
	{
		const auto& frame = _active_frames.top();
		post_error(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_error(ErrorContext* context, const std::string& file, const std::string& message, const Token& token)
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


	void ErrorReporter::post_uncorrectable(const std::string& message, const Token& token)
	{
		const auto& frame = _active_frames.top();
		post_uncorrectable(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_uncorrectable(ErrorContext* context, const std::string& file, const std::string& message, const Token& token)
	{
		_error_count++;
		// not checking validating the error count here since that would be recursive!

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