#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include <allocator/RuntimeAllocator.h>
#include <data/DependencyInjector.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>
#include <toolchain/Parser.h>

// Haze Function.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Expression;
	class Statement;
	class Type;

	class Function 
		: public InjectService<Generator, Parser, RuntimeAllocator>
		, public InjectSingleton<SymbolDatabase>
	{
	public:
		std::string_view name;
		Type* return_type;
		std::vector<Expression*> arguments;
		Statement* body;

	public:
		Function(std::string_view, Type*, const std::vector<Expression*>&, Statement*, const Token&)

	public:
		virtual NodeType ntype() const final override;
		virtual void generate(Allocation*) final override;
		virtual Function* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
