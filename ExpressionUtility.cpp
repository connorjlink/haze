import std;

#include "ExpressionUtility.h"
#include "IdentifierExpression.h"
#include "Parser.h"
#include "Symbol.h"

// Haze ExpressionUtility.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	bool is_mapped_identifier(Expression* expression)
	{
		if (expression->etype() == ExpressionType::IDENTIFIER)
		{
			auto identifier_expression = AS_IDENTIFIER_EXPRESSION(expression);

			if (_parser->has_symbol(identifier_expression->name))
			{
				auto symbol_type = _parser->query_symbol_type(identifier_expression->name, NULL_TOKEN);

				if (symbol_type == SymbolType::VARIABLE)
				{
					auto symbol = _parser->reference_symbol(SymbolType::VARIABLE, identifier_expression->name, NULL_TOKEN);
					auto variable_symbol = AS_VARIABLE_SYMBOL(symbol);

					// does the referenced variable have a register mapping currently?
					if (variable_symbol->allocation != nullptr)
					{
						return true;
					}
				}
			}
		}

		return false;
	}
}
