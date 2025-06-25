#ifndef HAZE_SYMBOL_EXPORTER_H
#define HAZE_SYMBOL_EXPORTER_H

// Haze SymbolExporter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class SymbolExporter
	{
	private:
		std::queue<Symbol*> _queue;

	public:
		void produce(Symbol*) noexcept;
	};
}

#endif
