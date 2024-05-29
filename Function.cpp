#include "Function.h"
#include "Generator.h"
#include "Allocator.h"

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


	void Function::generate(Allocation*)
	{
		generator->begin_function(name);
		body->generate();
		generator->exit();
	}


	Function* Function::optimize()
	{
		if (auto body_optimized = body->optimize())
		{
			return new Function{ name, return_type, std::move(arguments), AS_STATEMENT(body_optimized) };
		}

		return nullptr;
	}
}
