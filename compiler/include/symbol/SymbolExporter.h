#ifndef HAZE_SYMBOL_EXPORTER_H
#define HAZE_SYMBOL_EXPORTER_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <io/WebSocketClient.h>
#undef VOID // windows lol
#include <toolchain/models/Token.h>

// Haze SymbolExporter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Symbol;

	class SymbolExporter 
		: public SingletonTag<SymbolExporter>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		struct QueueEntry
		{
			Symbol* symbol;
			Token token;
		};

	private:
		std::queue<QueueEntry> _queue;
		std::mutex _queue_mutex;
		std::condition_variable_any _listener;
		std::jthread _worker;

	private:
		// optional to allow dynamic disablement
		std::optional<std::osyncstream> _stream;
		std::optional<WebSocketClient> _client;
		std::wstring _path;

	// producer operations
	public:
		void enqueue(Symbol*, Token);

	// consumer operations
	public:
		void launch();
		void notify();

	public:
		void try_reconnect(int);
	
	private:
		void export_symbol(QueueEntry);

	public:
		SymbolExporter(std::ostream&);
		SymbolExporter(const std::wstring&);
		~SymbolExporter();
	};
}

#endif
