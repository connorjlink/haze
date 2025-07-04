import std;

#include <symbol/Symbol.h>
#include <symbol/SymbolExporter.h>
#include <utility/AutoJSON.h>
#include <utility/JSON.h>

// Haze SymbolExporter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	std::string export_function_symbol(FunctionSymbol* function_symbol, Token token)
	{
		/*
			{
				"name": "function",
				"value": null,
				"type": "keyword",
				"text": "function",
				"location": {
					"line": 1,
					"column": 1,
					"filepath": "haze.hz",
					"stage": "source"
				},
				"confidence": 100
			}
			
		*/

		AutoJSONObject function{};
		function.upsert("name", new StringJSONValue{ function_symbol->name });
		function.upsert("value", new NullJSONValue{});
		function.upsert("type", new StringJSONValue{ "function" });
		function.upsert("text", new StringJSONValue{ function_symbol->name });

		AutoJSONObject location{};
		location.upsert("position", new NumberJSONValue{ static_cast<std::int32_t>(token.location.position) });
		location.upsert("line", new NumberJSONValue{ token.location.line });
		location.upsert("column", new NumberJSONValue{ token.location.column });
		location.upsert("filepath", new StringJSONValue{ token.location.filepath });
		location.upsert("stage", new StringJSONValue{ "source" });

		function.upsert("location", location.get_object());
		function.upsert("confidence", new NumberJSONValue{ function_symbol->confidence });

		return function.serialize();
	}

	std::string export_argument_symbol(ArgumentSymbol* argument_symbol, Token token)
	{
		return "";
	}

	std::string export_variable_symbol(VariableSymbol* variable_symbol, Token token)
	{
		return "";
	}

	std::string export_define_symbol(DefineSymbol* define_symbol, Token token)
	{
		return "";
	}

	std::string export_label_symbol(LabelSymbol* label_symbol, Token token)
	{
		return "";
	}

	std::string export_struct_symbol(StructSymbol* struct_symbol, Token token)
	{
		return "";
	}
}

namespace hz
{
	void SymbolExporter::enqueue(Symbol* symbol, Token token)
	{
		// lock
		{
			std::scoped_lock lock{ _queue_mutex };
			_queue.push({ symbol, token });
		}
		
		notify();
	}

	void SymbolExporter::launch()
	{
		_worker = std::jthread([this](std::stop_token stop_token)
		{
			while (true)
			{
				QueueEntry entry{};
				{
					std::unique_lock lock{ _queue_mutex };
					_listener.wait(lock, stop_token, [this] { return !_queue.empty(); });

					if (stop_token.stop_requested() && _queue.empty())
						break;

					if (_queue.empty())
						continue;

					entry = _queue.front();
					_queue.pop();
				}

				if (entry.symbol != nullptr)
				{
					export_symbol(entry);
				}
			}
		});
	}

	void SymbolExporter::notify()
	{
		_listener.notify_one();
	}

	void SymbolExporter::export_symbol(QueueEntry entry)
	{
		const auto symbol = entry.symbol;
		const auto location = entry.token;

		using enum SymbolType;
		switch (symbol->ytype())
		{
			case FUNCTION:
				*_stream << ::export_function_symbol(AS_FUNCTION_SYMBOL(symbol), location);
				break;

			case ARGUMENT:
				*_stream << ::export_argument_symbol(AS_ARGUMENT_SYMBOL(symbol), location);
				break;
			
			case VARIABLE:
				*_stream << ::export_variable_symbol(AS_VARIABLE_SYMBOL(symbol), location);
				break;

			case DEFINE:
				*_stream << ::export_define_symbol(AS_DEFINE_SYMBOL(symbol), location);
				break;

			case LABEL:
				*_stream << ::export_label_symbol(AS_LABEL_SYMBOL(symbol), location);
				break;

			case STRUCT:
				*_stream << ::export_struct_symbol(AS_STRUCT_SYMBOL(symbol), location);
				break;
		}

		_stream->emit();
	}

	void SymbolExporter::try_reconnect(int retry_attempts)
	{
		// not considering running out of attempts an error because it might just be that the server gracefully shut down
		if (retry_attempts == 0)
		{
			return;
		}

		_client->connect(L"http://localhost:8080", L"");

		_client->on_open = [&]
		{
			// perhaps transmit some handshake information?
		};

		_client->on_message = [&](const std::string& message)
		{
			// TODO: talk to the toolchain to iniciate appropriate recompilation
			// probably a full build for now of the specified path, but perhaps an incremental one might make sense
			// I think the source would have to get passed through with the message for this
			// Otherwise it would have to re-read from disk and that won't work once the server is a webworker

			// I believe that in all cases this should represent a symbol/recompile request
		};

		_client->on_close = [&]
		{
			try_reconnect(retry_attempts - 1);
		};

		_client->on_error = [&](const std::string& error)
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"websocket error `{}`", error), NULL_TOKEN);
		};
	}

	SymbolExporter::SymbolExporter(std::ostream& stream)
		: _client{ std::nullopt }, _stream{ std::osyncstream{ stream } }
	{
		// locking machinery auto-initialized
		_queue = {};
	}

	SymbolExporter::SymbolExporter(const std::wstring& path)
		: _client{}, _stream{ std::nullopt }
	{
		// locking machinery auto-initialized
		_queue = {};

		_path = path;
		try_reconnect(3);
	}

	SymbolExporter::~SymbolExporter()
	{
		if (_worker.joinable())
		{
			_worker.request_stop();
		}

		_listener.notify_all();
	}
}
