#ifndef HAZE_SYMBOL_EXPORTER_H
#define HAZE_SYMBOL_EXPORTER_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <io/WebSocketClient.h>
#include <symbol/Symbol.h>
#include <toolchain/models/Token.h>
#include <utility/Sum.h>

// Haze SymbolExporter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	FORWARD_DECLARE_SUM(Declaration)
	FORWARD_DECLARE_SUM(Function)
	FORWARD_DECLARE_SUM(Statement)
	FORWARD_DECLARE_SUM(Expression)
	FORWARD_DECLARE_SUM(Type)


	struct QueueEntry;

	struct SymbolExporter 
		: public SingletonTag<SymbolExporter>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		std::list<QueueEntry> queue;
		std::mutex queue_mutex;
		std::condition_variable_any listener;
		std::jthread worker;

	private:
		// optional to allow dynamic disablement
		std::osyncstream stream;
		WebSocketClient client;
		std::filesystem::path path;

	// producer operations
	public:
		void enqueue(Symbol, Token);

	// consumer operations
	public:
		void launch();
		void notify();

	public:
		void try_reconnect(std::uint8_t);
	
	private:
		void export_symbol(QueueEntry);

	public:
		SymbolExporter(std::ostream&);
		~SymbolExporter();
	};
}

#endif
