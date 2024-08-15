#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

namespace hz
{
	using VariableType = int;
	
	// Required to avoid a dependency between Function and Context
	class Function;

	// Required to avoid a dependency between Expression and Context
	class Expression;

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
