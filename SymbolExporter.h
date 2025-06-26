#ifndef HAZE_SYMBOL_EXPORTER_H
#define HAZE_SYMBOL_EXPORTER_H

// Haze SymbolExporter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Symbol;
	class Token;

	class SymbolExporter
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
		std::osyncstream _stream;

	// producer operations
	public:
		void enqueue(Symbol*, Token);

	// consumer operations
	public:
		void launch();
		void notify();
	
	private:
		void export_symbol(QueueEntry);

	public:
		SymbolExporter(std::ostream&);
		~SymbolExporter();
	};

	extern SymbolExporter* _exporter;
}

#endif
