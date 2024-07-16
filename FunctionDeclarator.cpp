#include "FunctionDeclarator.h"

#include <format>

namespace hz
{
	Declarator::Type FunctionDeclarator::dtype() const
	{
		return Declarator::Type::FUNCTION;
	}

	std::string FunctionDeclarator::string() const
	{
		return std::format("function declarator ({})", function->name);
	}

	FunctionDeclarator* FunctionDeclarator::copy() const
	{
		return new FunctionDeclarator{ *this };
	}

	void FunctionDeclarator::generate(Allocation*)
	{
		// TODO: nothing to do here??

		// A possible option is to re-utilize generate() as 
		// an equivalent to evaluate() for the interpreter context!
	}

	Declarator* FunctionDeclarator::optimize()
	{
		auto function_body_optimized = AS_STATEMENT(function->body->optimize());

		if (function_body_optimized != nullptr)
		{
			auto function_copy = new Function{ *function };
			function_copy->body = function_body_optimized;
			return new FunctionDeclarator{ function_copy };
		}

		return nullptr;
	}
}