#ifndef HAZE_CONTEXT_H
#define HAZE_CONTEXT_H

#include <string>
#include <vector>
#include <stack>

namespace hz
{
	using VariableType = int;

	struct Variable
	{
		std::string _name;
		VariableType _value;
	};

	struct Context
	{
		std::vector<Variable> _variables;
		std::stack<VariableType> _returns;
	};

	extern Context* _context;
}

#endif
