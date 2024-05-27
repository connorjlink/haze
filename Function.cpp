#include "Function.h"
#include "Generator.h"

#include <format>

namespace hz
{
	Node::Type Function::ntype() const
	{
		return Node::Type::FUNCTION;
	}


	std::string Function::string() const
	{
		return std::format("function ({}())", name);
	}


	Function* Function::copy() const
	{
		return new Function{ *this };
	}


	void Function::generate(Allocation* allocation)
	{
		generator->begin_function();
		body->generate(allocation);
	}


	Function* Function::optimize()
	{
		if (auto body_optimized = body->optimize())
		{
			return new Function{ name, std::move(arguments), AS_STATEMENT(body_optimized) };
		}

		return nullptr;
	}
}
