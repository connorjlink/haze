import std;

#include "Symbol.h"
#include "SymbolExporter.h"
#include "SymbolDatabase.h"
#include "AutoJSON.h"
#include "JSON.h"

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
		location.upsert("line", new NumberJSONValue{ token.line });
		location.upsert("column", new NumberJSONValue{ token.column });
		location.upsert("filepath", new StringJSONValue{ token.filepath });
		location.upsert("stage", new StringJSONValue{ "source" });

		function.upsert("location", location.get_object());
		function.upsert("confidence", new NumberJSONValue{ function_symbol->confidence });

		return function.serialize();
	}

	void export_argument_symbol(ArgumentSymbol* argument_symbol, Token token)
	{

	}

	void export_variable_symbol(VariableSymbol* variable_symbol, Token token)
	{

	}

	void export_define_symbol(DefineSymbol* define_symbol, Token token)
	{

	}

	void export_label_symbol(LabelSymbol* label_symbol, Token token)
	{

	}

	void export_struct_symbol(StructSymbol* struct_symbol, Token token)
	{

	}
}

namespace hz
{
	// GLOBALS
	SymbolExporter* _exporter;
	// GLOBALS


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

		auto& stream = std::cout;

		using enum SymbolType;
		switch (symbol->ytype())
		{
			case FUNCTION:
				stream << ::export_function_symbol(AS_FUNCTION_SYMBOL(symbol), location);
				break;

			case ARGUMENT:
				::export_argument_symbol(AS_ARGUMENT_SYMBOL(symbol), location);
				break;
			
			case VARIABLE:
				::export_variable_symbol(AS_VARIABLE_SYMBOL(symbol), location);
				break;

			case DEFINE:
				::export_define_symbol(AS_DEFINE_SYMBOL(symbol), location);
				break;

			case LABEL:
				::export_label_symbol(AS_LABEL_SYMBOL(symbol), location);
				break;

			case STRUCT:
				::export_struct_symbol(AS_STRUCT_SYMBOL(symbol), location);
				break;
		}
	}

	SymbolExporter::SymbolExporter(std::ostream& stream)
		: _stream{ std::osyncstream{ stream } }
	{
		// locking machinery auto-initialized
		_queue = {};
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
