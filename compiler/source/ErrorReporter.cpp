import std;

#include <error/ErrorContext.h>
#include <error/ErrorReporter.h>
#include <toolchain/Toolchain.h>

// Haze ErrorReporter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void ErrorReporter::close_these_contexts(std::thread::id thread_id)
	{
		while (_active_frames[thread_id].size() > 0)
		{
			close_context();
		}
	}

	void ErrorReporter::close_these_contexts()
	{
		const auto _ = std::scoped_lock{ _mutex };

		const auto thread_id = std::this_thread::get_id();
		close_these_contexts(thread_id);
	}

	void ErrorReporter::close_all_contexts()
	{
		const auto _ = std::scoped_lock{ _mutex };
		
		for (const auto& [thread_id, _] : _active_frames)
		{
			close_these_contexts(thread_id);
		}
	}

	std::size_t ErrorReporter::get_context_count(void)
	{
		const auto _ = std::scoped_lock{ _mutex };

		const auto thread_id = std::this_thread::get_id();
		return _active_frames[thread_id].size();
	}

	ErrorFrame ErrorReporter::open_context(const std::string& file, const std::string& task)
	{
		const auto _ = std::scoped_lock{ _mutex };

		const auto thread_id = std::this_thread::get_id();
		auto& existing_contexts = _open_frames[thread_id][file];
		auto* new_context = &existing_contexts.emplace_back(ErrorContext{ task });

		const ErrorFrame frame{ new_context, file };
		_active_frames[thread_id].emplace(frame);

		return { new_context, file };
	}

	void ErrorReporter::close_context()
	{
		const auto _ = std::scoped_lock{ _mutex };

		const auto thread_id = std::this_thread::get_id();
		_closed_frames[thread_id].emplace_back(_active_frames[thread_id].top());
		_active_frames[thread_id].pop();
	}

	std::string ErrorReporter::generate_report()
	{
		const auto _ = std::scoped_lock{ _mutex };

		std::string report(0x2000, '\0');

		// generate report for every thread
		for (auto& [_, frameset] : _closed_frames)
		{
			for (auto& frame : frameset)
			{
				if (frame.context->error_count() > 0)
				{
					report += std::format("in {}\n{}", frame.filepath, frame.context->format());
				}
			}
		}

		return report;
	}

	void ErrorReporter::post_information(const std::string& message, const Token& token)
	{
		const auto _ = std::scoped_lock{ _mutex };

		const auto thread_id = std::this_thread::get_id();
		const auto& frame = _active_frames[thread_id].top();
		post_information(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_information(ErrorContext* context, const std::string& file, const std::string& message, const Token& token)
	{
		const auto _ = std::scoped_lock{ _mutex };

		if (context != nullptr)
		{
			context->post(ErrorType::INFORMATION, message, file, token);
			return;
		}

		post_information(message, token);
	}

	void ErrorReporter::post_warning(const std::string& message, const Token& token)
	{
		const auto _ = std::scoped_lock{ _mutex };

		const auto thread_id = std::this_thread::get_id();
		const auto& frame = _active_frames[thread_id].top();
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
		const auto _ = std::scoped_lock{ _mutex };

		const auto thread_id = std::this_thread::get_id();
		const auto& frame = _active_frames[thread_id].top();
		post_error(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_error(ErrorContext* context, const std::string& file, const std::string& message, const Token& token)
	{
		const auto _ = std::scoped_lock{ _mutex };

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
		const auto _ = std::scoped_lock{ _mutex };

		const auto thread_id = std::this_thread::get_id();
		const auto& frame = _active_frames[thread_id].top();
		post_uncorrectable(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_uncorrectable(ErrorContext* context, const std::string& file, const std::string& message, const Token& token)
	{
		const auto _ = std::scoped_lock{ _mutex };

		// not checking validating the error count here since that would be recursive uncorrectable!
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
		REQUIRE_UNSAFE(Toolchain)->panic();
	}
}
