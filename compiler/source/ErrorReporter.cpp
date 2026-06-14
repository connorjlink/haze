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
		while (active_frames[thread_id].size() > 0)
		{
			close_context();
		}
	}

	void ErrorReporter::close_these_contexts()
	{
		std::scoped_lock lock{ mutex };

		const auto thread_id = std::this_thread::get_id();
		close_these_contexts(thread_id);
	}

	void ErrorReporter::close_all_contexts()
	{
		std::scoped_lock lock{ mutex };
		
		for (const auto& [thread_id, _] : active_frames)
		{
			close_these_contexts(thread_id);
		}
	}

	std::size_t ErrorReporter::get_context_count()
	{
		std::scoped_lock lock{ mutex };

		const auto thread_id = std::this_thread::get_id();
		return active_frames[thread_id].size();
	}

	ErrorFrame ErrorReporter::open_context(std::filesystem::path filepath, std::string_view task)
	{
		std::scoped_lock lock{ mutex };

		const auto thread_id = std::this_thread::get_id();
		auto& existing_contexts = open_frames[thread_id][filepath];
		auto* new_context = &existing_contexts.emplace_back(ErrorContext{ task });

		const auto frame = ErrorFrame{ new_context, filepath };
		active_frames[thread_id].emplace(frame);

		return { new_context, filepath };
	}

	void ErrorReporter::close_context()
	{
		std::scoped_lock lock{ mutex };

		const auto thread_id = std::this_thread::get_id();
		closed_frames[thread_id].emplace_back(active_frames[thread_id].top());
		active_frames[thread_id].pop();
	}

	std::string ErrorReporter::generate_report()
	{
		std::scoped_lock lock{ mutex };

		std::string report(0x2000, '\0');

		// generate report for every thread
		for (auto& [_, frameset] : closed_frames)
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
		std::scoped_lock lock{ mutex };

		const auto thread_id = std::this_thread::get_id();
		const auto& frame = active_frames[thread_id].top();
		post_information(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_information(ErrorContext* context, const std::string& filepath, const std::string& message, const Token& token)
	{
		std::scoped_lock lock{ mutex };

		if (context != nullptr)
		{
			context->post(ErrorKind::INFORMATION, message, filepath, token);
			return;
		}

		post_information(message, token);
	}

	void ErrorReporter::post_warning(const std::string& message, const Token& token)
	{
		std::scoped_lock lock{ mutex };

		const auto thread_id = std::this_thread::get_id();
		const auto& frame = active_frames[thread_id].top();
		post_warning(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_warning(ErrorContext* context, const std::string& filepath, const std::string& message, const Token& token)
	{
		if (context != nullptr)
		{
			context->post(ErrorKind::WARNING, message, filepath, token);
			return;
		}

		post_warning(message, token);
	}

	void ErrorReporter::post_error(const std::string& message, const Token& token)
	{
		std::scoped_lock lock{ mutex };

		const auto thread_id = std::this_thread::get_id();
		const auto& frame = active_frames[thread_id].top();
		post_error(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_error(ErrorContext* context, const std::string& filepath, const std::string& message, const Token& token)
	{
		std::scoped_lock lock{ mutex };

		error_count++;
		validate_error_count();

		if (context != nullptr)
		{
			context->post(ErrorKind::ERROR, message, filepath, token);
			return;
		}

		post_error(message, token);
	}

	void ErrorReporter::post_uncorrectable(const std::string& message, const Token& token)
	{
		std::scoped_lock lock{ mutex };

		const auto thread_id = std::this_thread::get_id();
		const auto& frame = active_frames[thread_id].top();
		post_uncorrectable(frame.context, frame.filepath, message, token);
	}

	void ErrorReporter::post_uncorrectable(ErrorContext* context, const std::string& filepath, const std::string& message, const Token& token)
	{
		std::scoped_lock lock{ mutex };

		// not checking validating the error count here since that would be recursive uncorrectable!
		error_count++;

		if (context != nullptr)
		{
			context->post(ErrorKind::UNCORRECTABLE, message, filepath, token);
		}
		else
		{
			post_uncorrectable(message, token);
		}
		
		close_all_contexts();
		REQUIRE_UNSAFE(Toolchain)->panic();
	}
}
