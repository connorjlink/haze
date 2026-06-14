import std;

#include <symbol/SymbolExporter.h>
#include <utility/JSON.h>
#include <ast/function/Function.h>

// Haze SymbolExporter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	JSON export_location(const Token& token)
	{
		auto result = JSONObject{};

		result.emplace_back("position", static_cast<std::int32_t>(token.location.position));
		result.emplace_back("line", token.location.line);
		result.emplace_back("column", token.location.column);
		result.emplace_back("filepath", token.location.filepath.string());
		result.emplace_back("stage", "source");
		
		return result;
	}

	JSON export_function_symbol(const Function& function, const Token& token)
	{
		auto result = JSONObject{};

		result.emplace_back("name", function.name);
		result.emplace_back("value", nullptr);
		result.emplace_back("type", "function");
		result.emplace_back("text", function.format_signature());
		result.emplace_back("location", export_location(token));
		result.emplace_back("confidence", token.confidence);

		return result;
	}


	std::string export_variable_symbol(VariableSymbol* variable_symbol, const Token& token)
	{
#pragma message("TODO: implement the rest of the export functions")
		return "";
	}

	std::string export_define_symbol(DefineSymbol* define_symbol, const Token& token)
	{
		return "";
	}

	std::string export_label_symbol(LabelSymbol* label_symbol, const Token& token)
	{
		return "";
	}

	std::string export_struct_symbol(StructOrUnionSymbol* struct_symbol, const Token& token)
	{
		return "";
	}
}

namespace hz
{
	void SymbolExporter::enqueue(Symbol symbol, const Token& token)
	{
		// lock
		{
			std::scoped_lock lock{ queue_mutex };
			queue.emplace(symbol, token);
		}
		
		notify();
	}

	void SymbolExporter::launch()
	{
		worker = std::jthread([this](std::stop_token stop_token)
		{
			while (true)
			{
				auto result = std::optional<QueueEntry>{};
				{
					std::unique_lock lock{ queue_mutex };
					listener.wait(lock, stop_token, [this]
					{ 
						return !queue.empty();
					});

					if (stop_token.stop_requested() && queue.empty())
					{
						break;
					}

					if (queue.empty())
					{
						continue;
					}

					result = queue.front();
					queue.pop();
				}

				if (result)
				{
					export_symbol(*result);
				}
			}
		});
	}

	void SymbolExporter::notify()
	{
		listener.notify_one();
	}

	void SymbolExporter::export_symbol(QueueEntry entry)
	{
		const auto symbol = entry.symbol;
		const auto& location = entry.token;

		using enum SymbolKind;
		switch (symbol->ytype())
		{
			case FUNCTION:
				*stream << ::export_function_symbol(AS_FUNCTION_SYMBOL(symbol), location);
				break;

			case ARGUMENT:
				*stream << ::export_argument_symbol(AS_ARGUMENT_SYMBOL(symbol), location);
				break;
			
			case VARIABLE:
				*stream << ::export_variable_symbol(AS_VARIABLE_SYMBOL(symbol), location);
				break;

			case DEFINE:
				*stream << ::export_define_symbol(AS_DEFINE_SYMBOL(symbol), location);
				break;

			case LABEL:
				*stream << ::export_label_symbol(AS_LABEL_SYMBOL(symbol), location);
				break;

			case STRUCT:
				*stream << ::export_struct_symbol(AS_STRUCT_SYMBOL(symbol), location);
				break;
		}

		stream.emit();
	}

	void SymbolExporter::try_reconnect(std::uint8_t retry_attempts)
	{
		// not considering running out of attempts an error because it might just be that the server gracefully shut down
		if (retry_attempts == 0)
		{
			USE_SAFE(ErrorReporter)->post_information(
				"failed to connect to the server after multiple attempts; symbol information will not be exported", NULL_TOKEN);
			return;
		}

		client.connect(L"http://localhost:8080", L"");

		client.on_open = [&]
		{
			// perhaps transmit some handshake information?
		};

		client.on_message = [&](const std::string& message)
		{
#pragma message("TODO: talk to the toolchain to iniciate appropriate recompilation")
			// probably a full build for now of the specified path, but perhaps an incremental one might make sense
			// I think the source would have to get passed through with the message for this
			// Otherwise it would have to re-read from disk and that won't work once the server is a webworker

			// I believe that in all cases this should represent a symbol/recompile request
		};

		client.on_close = [&]
		{
			try_reconnect(retry_attempts - 1);
		};

		client.on_error = [&](const std::string& error)
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"websocket error `{}`", error), NULL_TOKEN);
		};
	}

	SymbolExporter::SymbolExporter(std::ostream& stream)
		: queue{}, client{}, stream{ std::osyncstream{ stream } }
	{
		// locking machinery auto-initialized
		try_reconnect(3);
	}

	SymbolExporter::~SymbolExporter()
	{
		if (worker.joinable())
		{
			worker.request_stop();
		}

		listener.notify_all();
	}
}
