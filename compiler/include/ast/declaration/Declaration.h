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
	// forward declare sum storage and self-referential types for facade

	FORWARD_DECLARE_SUM(Declaration)

	template<typename MethodsT>
	using DeclarationASTMethods = BaseASTMethods<MethodsT, DeclarationHandle>;

	DEFINE_SUM(Declaration, BASE_AST_METHODS)


	struct DeclarationBase 
		: public DeclarationFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using Storage = DeclarationSumStorage;

	public:
		Token token;

	public:
		DeclarationBase(const Token& token)
			: token{ token }
		{
		}

	public:
		template<typename Self>
		DeclarationKind declaration_kind(this Self&&);
	};
}

namespace hz
{
#pragma message("TODO: implement remaining declaration AST nodes")

	struct FunctionDeclaration : public DeclarationBase
	{
	private:
		TypeReference<FunctionType> type;
		std::vector<std::string> formal_parameters;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		DeclarationHandle evaluate(const Storage&, Context&) const;
		DeclarationHandle optimize(const Storage&) const;

	public:
		FunctionDeclaration(const Token& token, TypeReference<FunctionType> type, const std::vector<std::string>& formal_parameters)
			: DeclarationBase{ token }, type{ type }, formal_parameters{ formal_parameters }
		{
		}
	};


	struct VariableDeclaration : public DeclarationBase
	{
	private:
		TypeHandle type;
		ExpressionHandle initializer;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		DeclarationHandle evaluate(const Storage&, Context&) const;
		DeclarationHandle optimize(const Storage&) const;

	public:
		VariableDeclaration(const Token& token, TypeHandle type, ExpressionHandle initializer)
			: DeclarationBase{ token }, type{ type }, initializer{ initializer }
		{
		}
	};


	struct StructOrUnionDeclaration : public DeclarationBase
	{
	private:
		TypeReference<StructOrUnionType> type;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		DeclarationHandle evaluate(const Storage&, Context&) const;
		DeclarationHandle optimize(const Storage&) const;

	public:
		StructOrUnionDeclaration(const Token& token, TypeReference<StructOrUnionType> type)
			: DeclarationBase{ token }, type{ type }
		{
		}
	};

	struct EnumDeclaration : public DeclarationBase
	{
	private:
		TypeReference<EnumType> type;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		DeclarationHandle evaluate(const Storage&, Context&) const;
		DeclarationHandle optimize(const Storage&) const;

	public:
		EnumDeclaration(const Token& token, TypeReference<EnumType> type)
			: DeclarationBase{ token }, type{ type }
		{
		}
	};

	struct TypedefDeclaration : public DeclarationBase
	{
	private:
		TypeReference<TypedefNameType> type;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		DeclarationHandle evaluate(const Storage&, Context&) const;
		DeclarationHandle optimize(const Storage&) const;

	public:
		TypedefDeclaration(const Token& token, TypeReference<TypedefNameType> type)
			: DeclarationBase{ token }, type{ type }
		{
		}
	};


	//////////////////////////////////////////////////////
	// All Declarations
	//////////////////////////////////////////////////////

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept IsDeclaration = SumTuple<SumMemberT, SumStorageT, DeclarationASTMethods<SumStorageT>>;

	using DeclarationKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		DECLARATION_KINDS(X)
#undef X
		void
	>;

	using DeclarationSumImplementation = MakeSum<DeclarationASTMethods, DeclarationKinds>::Type;

	struct DeclarationSumStorage : public DeclarationSumImplementation::Storage
	{
		using DeclarationSumImplementation::Storage::Storage;

		using Type = DeclarationSumImplementation::Type;
		using Anchor = DeclarationSumImplementation::Anchor;
	};


	template<typename Self>
	DeclarationKind DeclarationBase::declaration_kind(this Self&& self)
	{
		switch (self.tag_type())
		{
#define X(enumerator, type, name) case TypeIndexV<type, typename DeclarationSumStorage::Type>: return DeclarationKind::enumerator;
			DECLARATION_KINDS(X)
#undef X
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid declaration tag `{}`", self.tag_type()), self.token);

		// error recovery does not care about declaration kind
		return DeclarationKind::FUNCTION;
	}
}

#endif
