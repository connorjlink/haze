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

	std::string format_type(TypeHandle type, const std::string& name = "<anonymous>", TypePrecedence parent_predence = TypePrecedence::LOWEST)
	{
		switch (type.type_kind())
		{
			case TypeKind::VOID:
			{
				return "void";
			} break;

			case TypeKind::INT:
			{
				const auto& int_type = type.get<IntType>();

				// <storage-class> <type-qualifier> <type-specifier> <type-width>
				return std::format("{} {} {} {}",
					int_type.storage,
					int_type.qualifier,
					int_type.signedness,
					int_type.int_kind);
			} break;

			case TypeKind::FLOAT:
			{
				const auto& float_type = type.get<FloatType>();

				// <storage-class> <type-qualifier> <type-width>
				return std::format("{} {} {}",
					float_type.storage,
					float_type.qualifier,
					float_type.float_kind);
			} break;

			case TypeKind::STRUCT_OR_UNION:
			{
				const auto& struct_or_union_type = type.get<StructOrUnionType>();

				if (!struct_or_union_type.members.has_value())
				{
					CommonErrors::invalid_struct_or_union_type(struct_or_union_type.struct_or_union_kind, NULL_TOKEN);
					return "";
				}

				std::string members{};
				for (const auto& member : struct_or_union_type.members.value())
				{
					members += std::format("    {} {};\n", format_type(member.type), member.name);
				}

				// <storage-class> <type-qualifier> <struct-or-union> 
				// {
				//     members...
				// }
				return std::format("{} {} {} {{\n{}}}",
					struct_or_union_type.storage,
					struct_or_union_type.qualifier,
					struct_or_union_type.struct_or_union_kind,
					members);
			} break;

			case TypeKind::ENUM:
			{
				const auto& enum_type = type.get<EnumType>();

				// <storage-class> <type-qualifier> enum tag 
				// NOTE: enumerators not shown since they don't fundamentally affect the type
				return std::format("{} {} enum {}",
					enum_type.storage,
					enum_type.qualifier,
					name);
			} break;

			case TypeKind::TYPEDEF_NAME:
			{
				const auto& typedef_name_type = type.get<TypedefNameType>();

				// NOTE: just using the underyling alias name since the typedef doesn't affect the type's string representation
				return typedef_name_type.name;
			} break;

			case TypeKind::POINTER:
			{
				const auto& pointer_type = type.get<PointerType>();
				const auto qualifier_string = format_type_qualifier(pointer_type.qualifier);

				// <pointee-type>* <pointer-qualifiers>
				auto pointer_string = std::format("*{}{}{}",
					(qualifier_string.empty() ? "" : " "),
					qualifier_string,
					name);

				if (parent_predence > TypePrecedence::POINTER && parent_predence != TypePrecedence::LEAF)
				{
					pointer_string = std::format("({})", pointer_string);
				}

				return format_type(pointer_type.pointee, pointer_string, TypePrecedence::POINTER);
			} break;

			case TypeKind::ARRAY:
			{
				const auto& array_type = type.get<ArrayType>();
				const auto array_length = array_type.length.has_value()
					? std::to_string(array_type.length.value())
					: "";

				// <element-type> [ <array-size> ]
				const auto name_string = std::format("{}[{}]", name, array_length);

				return format_type(array_type.element_type, name_string, TypePrecedence::ARRAY);
			} break;

			case TypeKind::FUNCTION:
			{
				const auto& function_type = type.get<FunctionType>();

				auto parameters = function_type.parameters
					| std::views::transform([](const auto& parameter)
						{
							return format_type(parameter, "", TypePrecedence::FUNCTION);
						})
					| std::views::join(std::string(", "))
					| std::ranges::to<std::string>();

				if (function_type.is_variadic)
				{
					if (!parameters.empty())
					{
						parameters += ", ";
					}

					parameters += "...";
				}

				const auto name_string = std::format("{}({})", name, parameters);
				return format_type(function_type.return_type, name_string, TypePrecedence::FUNCTION);
			}
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(
			"invalid type for string formatting", NULL_TOKEN);
		return "";
	}
}
