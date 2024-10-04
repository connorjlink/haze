#ifndef HAZE_SYMBOLTYPE_H
#define HAZE_SYMBOLTYPE_H

// Haze SymbolType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_FUNCTION_SYMBOL(x) static_cast<FunctionSymbol*>(x)
#define AS_ARGUMENT_SYMBOL(x) static_cast<ArgumentSymbol*>(x)
#define AS_VARIABLE_SYMBOL(x) static_cast<VariableSymbol*>(x)
#define AS_DEFINE_SYMBOL(x) static_cast<DefineSymbol*>(x)
#define AS_LABEL_SYMBOL(x) static_cast<LabelSymbol*>(x)
#define AS_STRUCT_SYMBOL(x) static_cast<StructSymbol*>(x)

namespace hz
{
	enum class SymbolType
	{
		FUNCTION,
		ARGUMENT,
		VARIABLE,
		DEFINE,
		LABEL,
		STRUCT,
	};

	static const std::unordered_map<SymbolType, std::string_view> _symbol_map
	{
		{ SymbolType::FUNCTION, "function" },
		{ SymbolType::ARGUMENT, "argument" },
		{ SymbolType::VARIABLE, "variable" },
		{ SymbolType::DEFINE, "define" },
		{ SymbolType::LABEL, "label" },
		{ SymbolType::STRUCT, "struct" },
	};
}

#endif 
