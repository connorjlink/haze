#ifndef HAZE_DECLARATION_H
#define HAZE_DECLARATION_H

#include <ast/AST.h>
#include <ast/declaration/defs/DeclarationKind.h>
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


	struct DeclarationBase : public DeclarationFacade
	{
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

	class FunctionDeclaration : public DeclarationBase
	{
	};


	class VariableDeclaration : public DeclarationBase
	{

	};


	class StructOrUnionDeclaration : public DeclarationBase
	{
	
	};

	class EnumDeclaration : public DeclarationBase
	{
	
	};

	class TypedefDeclaration : public DeclarationBase
	{

	};



	//////////////////////////////////////////////////////
	// All Declarations
	//////////////////////////////////////////////////////

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept DeclarationConcept = SumTuple<SumMemberT, SumStorageT, DeclarationASTMethods<SumStorageT>>;

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
