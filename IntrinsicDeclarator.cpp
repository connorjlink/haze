#include "IntrinsicDeclarator.h"

#include <format>

namespace hz
{
	Declarator::Type IntrinsicDeclarator::dtype() const
	{
		return Declarator::Type::INTRINSIC;
	}

	std::string IntrinsicDeclarator::string() const
	{
		return std::format("intrinsic declarator {} ({})", name, intrinsic->string());
	}

	IntrinsicDeclarator* IntrinsicDeclarator::copy() const
	{
		return new IntrinsicDeclarator{ *this };
	}

	void IntrinsicDeclarator::generate()
	{
		// TODO: nothing to do here??
	}

	Declarator* IntrinsicDeclarator::optimize()
	{
		return nullptr;
	}
}