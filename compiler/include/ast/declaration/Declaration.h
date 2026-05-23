#ifndef HAZE_DECLARATION_H
#define HAZE_DECLARATION_H

// Haze Declaration.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	struct DeclarationSumStorage;

	using DeclarationHandle = SumHandle<DeclarationSumStorage>;

	template<typename T>
	using DeclarationReference = SumReference<T, DeclarationSumStorage>;

	using DeclarationFacade = SumMemberBase<DeclarationSumStorage>;

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
		DeclarationKind declaration_kind(this Self&& self)
		{
			switch (self.ttype())
			{
#define X(enumerator, type, name) case TypeIndexV<type, typename DeclarationSumStorage::Type>: return DeclarationKind::enumerator;
#include <ast/declaration/defs/DeclarationKind.def>
#undef X
			}

			return "<unknown declaration kind>";
		}
	};
}

#endif
