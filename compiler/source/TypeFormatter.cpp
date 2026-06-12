import std;

#include <type/Type.h>
#include <type/TypeFormatter.h>

// Haze TypeFormatter.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	TypePrecedence precedence(const TypeBase& type)
	{
		switch (type.type_kind())
		{
#define X(enumerator, precedence, type, name) case TypeKind::enumerator: return TypePrecedence::precedence;
			TYPE_KINDS(X)
#undef X
		}

		return TypePrecedence::LOWEST;
	}

	DECLARE_TYPE_FORMATTER(VoidType)
	{
		return "void";
	}

	DECLARE_TYPE_FORMATTER(IntType)
	{
		// <storage-struct> <type-qualifier> <type-specifier> <type-width>
		return std::format("{} {} {} {}",
			type.storage,
			type.qualifier,
			type.signedness,
			type.int_kind);
	}

	DECLARE_TYPE_FORMATTER(FloatType)
	{
		// <storage-struct> <type-qualifier> <type-width>
		return std::format("{} {} {}",
			type.storage,
			type.qualifier,
			type.float_kind);
	}

	DECLARE_TYPE_FORMATTER(StructOrUnionType)
	{
		if (!type.members.has_value())
		{
			CommonErrors::invalid_struct_or_union_type(type.struct_or_union_kind, NULL_TOKEN);
			return "";
		}

		std::string members{};
		for (const auto& member : type.members.value())
		{
			members += std::format("    {} {};\n", to_string(member.type), member.name);
		}

		// <storage-struct> <type-qualifier> <struct-or-union> 
		// {
		//     members...
		// }
		return std::format("{} {} {} {{\n{}}}",
			type.storage,
			type.qualifier,
			type.struct_or_union_kind,
			members);
	}

	DECLARE_TYPE_FORMATTER(EnumType)
	{
		// <storage-struct> <type-qualifier> enum tag 
		// NOTE: enumerators not shown since they don't fundamentally affect the type
		return std::format("{} {} enum {}",
			type.storage,
			type.qualifier,
			name);
	}

	DECLARE_TYPE_FORMATTER(TypedefNameType)
	{
		// NOTE: just using the underyling alias name since the typedef doesn't affect the type's string representation
		return type.name;
	}

	DECLARE_TYPE_FORMATTER(PointerType)
	{
		const auto qualifier_string = to_string(type.qualifier);

		// <pointee-type>* <pointer-qualifiers>
		auto pointer_string = std::format("*{}{}{}",
			(qualifier_string.empty() ? "" : " "),
			qualifier_string,
			name);

		if (parent_precedence > TypePrecedence::POINTER && parent_precedence != TypePrecedence::LEAF)
		{
			pointer_string = std::format("({})", pointer_string);
		}

		return to_string(type.pointee, pointer_string, TypePrecedence::POINTER);
	}

	DECLARE_TYPE_FORMATTER(ArrayType)
	{
		const auto array_length = type.length
			? std::to_string(*type.length)
			: "";

		// <element-type> [ <array-size> ]
		const auto name_string = std::format("{}[{}]", name, array_length);

		return to_string(type.element_type, name_string, TypePrecedence::ARRAY);
	}

	DECLARE_TYPE_FORMATTER(FunctionType)
	{
		auto parameters = std::string{};

		for (auto i = 0uz; i < type.parameters.size(); i++)
		{
			if (i != 0)
			{
				parameters += ", ";
			}

			parameters += to_string(type.parameters[i], "", TypePrecedence::FUNCTION);
		}

		if (type.is_variadic)
		{
			if (!parameters.empty())
			{
				parameters += ", ";
			}

			parameters += "...";
		}

		const auto name_string = std::format("{}({})", name, parameters);
		return to_string(type.return_type, name_string, TypePrecedence::FUNCTION);
	}

	std::string to_string(TypeHandle type, const std::string& name, TypePrecedence parent_precedence)
	{
		switch (type.type_kind())
		{
#define X(enumerator, precedence, t, n) case TypeKind::enumerator: \
			{ \
				const auto& derived_type = type.get<t>(); \
				return to_string(derived_type, name, parent_precedence); \
			} break;

			TYPE_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(
			"invalid type for string formatting", NULL_TOKEN);
		return "";
	}
}
