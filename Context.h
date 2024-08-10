#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

#include "Function.h"

namespace hz
{
	using VariableType = int;

	struct Context
	{
		std::unordered_map<std::string, VariableType> _variables;
		std::vector<Function*> _functions;
		std::stack<VariableType> _returns;
		std::stack<std::vector<Expression*>> _arguments;
	};

	extern Context* _context;
}

#endif
