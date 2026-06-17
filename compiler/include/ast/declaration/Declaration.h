#ifndef HAZE_DECLARATION_H
#define HAZE_DECLARATION_H

#include <ast/AST.h>
#include <ast/declaration/defs/DeclarationKind.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/Context.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Declaration.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	FORWARD_DECLARE_SUM(Declaration)

#define DECLARATION_AST_METHODS(X, handlet) \
	BASE_AST_METHODS(X, handlet) \
	X(declaration_kind, DeclarationKind)

	DEFINE_SUM(Declaration, DECLARATION_AST_METHODS)


	struct DeclarationBase 
		: public DeclarationFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using Storage = DeclarationStorage;

	public:
		Token token;

	public:
		DeclarationBase(const Token& token)
			: token{ token }
		{
		}
	};
}

namespace hz
{
	//////////////////////////////////////////////////////
	// AST Declarations
	//////////////////////////////////////////////////////

	struct FunctionDeclaration : public DeclarationBase
	{
	private:
		TypeReference<FunctionType> type;
		std::vector<std::string> formal_parameters;

	public:
		DeclarationKind declaration_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const DeclarationStorage&) const;
		DeclarationHandle evaluate(const DeclarationStorage&, Context&) const;
		DeclarationHandle optimize(const DeclarationStorage&) const;

	public:
		FunctionDeclaration(const Token& token, TypeReference<FunctionType> type, std::vector<std::string> formal_parameters)
			: DeclarationBase{ token }, type{ type }, formal_parameters{ std::move(formal_parameters) }
		{
		}
	};
#define MAKE_FUNCTION_DECLARATION(token, type, formal_parameters) FunctionDeclaration{ token, type, formal_parameters }

	struct VariableDeclaration : public DeclarationBase
	{
	private:
		TypeHandle type;
		ExpressionHandle initializer;

	public:
		DeclarationKind declaration_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const DeclarationStorage&) const;
		DeclarationHandle evaluate(const DeclarationStorage&, Context&) const;
		DeclarationHandle optimize(const DeclarationStorage&) const;

	public:
		VariableDeclaration(const Token& token, TypeHandle type, ExpressionHandle initializer)
			: DeclarationBase{ token }, type{ type }, initializer{ initializer }
		{
		}
	};
#define MAKE_VARIABLE_DECLARATION(token, type, initializer) VariableDeclaration{ token, type, make_handle(ast, initializer) }

	struct StructOrUnionDeclaration : public DeclarationBase
	{
	private:
		TypeReference<StructOrUnionType> type;

	public:
		DeclarationKind declaration_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const DeclarationStorage&) const;
		DeclarationHandle evaluate(const DeclarationStorage&, Context&) const;
		DeclarationHandle optimize(const DeclarationStorage&) const;

	public:
		StructOrUnionDeclaration(const Token& token, TypeReference<StructOrUnionType> type)
			: DeclarationBase{ token }, type{ type }
		{
		}
	};
#define MAKE_STRUCT_OR_UNION_DECLARATION(token, type) StructOrUnionDeclaration{ token, type }

	struct EnumDeclaration : public DeclarationBase
	{
	private:
		TypeReference<EnumType> type;

	public:
		DeclarationKind declaration_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const DeclarationStorage&) const;
		DeclarationHandle evaluate(const DeclarationStorage&, Context&) const;
		DeclarationHandle optimize(const DeclarationStorage&) const;

	public:
		EnumDeclaration(const Token& token, TypeReference<EnumType> type)
			: DeclarationBase{ token }, type{ type }
		{
		}
	};
#define MAKE_ENUM_DECLARATION(token, type) EnumDeclaration{ token, type }

	struct TypedefDeclaration : public DeclarationBase
	{
	private:
		TypeReference<TypedefNameType> type;

	public:
		DeclarationKind declaration_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const DeclarationStorage&) const;
		DeclarationHandle evaluate(const DeclarationStorage&, Context&) const;
		DeclarationHandle optimize(const DeclarationStorage&) const;

	public:
		TypedefDeclaration(const Token& token, TypeReference<TypedefNameType> type)
			: DeclarationBase{ token }, type{ type }
		{
		}
	};
#define MAKE_TYPEDEF_DECLARATION(token, type) TypedefDeclaration{ token, type }


	//////////////////////////////////////////////////////
	// All Declarations
	//////////////////////////////////////////////////////

	template<typename SumMemberT, typename StorageT>
	concept IsDeclaration = SumTuple<SumMemberT, StorageT, DeclarationMethods<typename StorageT::Anchor>>;

	using DeclarationKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		DECLARATION_KINDS(X)
#undef X
		void
	>;


	using DeclarationSumImplementation = MakeSum<DeclarationMethods, DeclarationKinds>::Type;

	struct DeclarationStorage : public DeclarationSumImplementation::Storage
	{
		using DeclarationSumImplementation::Storage::Storage;

		using Type = DeclarationSumImplementation::Type;
		using Anchor = DeclarationSumImplementation::Anchor;
	};
}

#endif
