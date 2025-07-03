#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include "Node.h"
#include "Generator.h"
#include "SymbolDatabase.h"
#include "RuntimeAllocator.h"
#include "Parser.h"
#include "DependencyInjector.h"

// Haze Function.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Expression;
	class Statement;
	class Type;

	class Function 
		: public Node
		, public InjectService<Generator, Parser, RuntimeAllocator>
		, public InjectSingleton<SymbolDatabase>
	{
	public:
		std::string name;
		Type* return_type;
		std::vector<Expression*> arguments;
		Statement* body;

	public:
		Function(const std::string&, Type*, std::vector<Expression*>&&, Statement*, const Token&);

	public:
		virtual NodeType ntype() const final override;
		virtual Function* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Function* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
