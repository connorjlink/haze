#ifndef HAZE_SYMBOLTYPE_H
#define HAZE_SYMBOLTYPE_H

#define AS_FUNCTION_SYMBOL(x) static_cast<FunctionSymbol*>(x)
#define AS_ARGUMENT_SYMBOL(x) static_cast<ArgumentSymbol*>(x)
#define AS_VARIABLE_SYMBOL(x) static_cast<VariableSymbol*>(x)
#define AS_DEFINE_SYMBOL(x) static_cast<DefineSymbol*>(x)
#define AS_LABEL_SYMBOL(x) static_cast<LabelSymbol*>(x)

#include <string>
#include <unordered_map>

namespace hz
{
	enum class SymbolType
	{
		FUNCTION,
		ARGUMENT,
		VARIABLE,
		DEFINE,
		LABEL,
	};

	static const std::unordered_map<SymbolType, std::string> _symbol_map
	{
		{ SymbolType::FUNCTION, "function" },
		{ SymbolType::ARGUMENT, "argument" },
		{ SymbolType::VARIABLE, "variable" },
		{ SymbolType::DEFINE, "define" },
		{ SymbolType::LABEL, "label" },
	};
}

#endif 
