#ifndef HAZE_TYPE_H
#define HAZE_TYPE_H

#include <data/DependencyInjector.h>
#include <symbol/SymbolDatabase.h>
#include <type/VoidType.h>
#include <type/IntType.h>
#include <type/FloatType.h>
#include <type/PointerType.h>
#include <type/FloatType.h>
#include <type/StructOrUnionType.h>
#include <type/EnumType.h>
#include <type/TypedefNameType.h>
#include <type/ArrayType.h>
#include <type/PointerType.h>
#include <type/FunctionType.h>
#include <type/StorageClass.h>
#include <type/TypeQualifier.h>
#include <type/TypeSignedness.h>
#include <utility/Sum.h>

// Haze Type.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// expose a strict polymorphic interface for types
	template<typename AnchorT>
	using TypeMethods = std::tuple
	<
		Method<&AnchorT::ttype, TypeKind()>,
		Method<&AnchorT::size, Offset()>,
		Method<&AnchorT::is_complete, bool()>,
	>;

	template<typename SumMemberT, typename SumStorageT>
	concept Type = SumTuple<SumMemberT, SumStorageT, TypeMethods<SumStorageT>>;

	class VoidType;
	class IntType;
	class FloatType;
	class StructOrUnionType;
	class EnumTypeKind;
	class TypedefNameType;
	class PointerType;
	class ArrayType;
	class FunctionType;

	using TypeKinds = SumTypeList
	<
		VoidType,
		IntType,
		FloatType,
		StructOrUnionType,
		EnumTypeKind,
		TypedefNameType,
		PointerType,
		ArrayType,
		FunctionType,
	>;

	using TypeSum = MakeSum<TypeMethods, TypeKinds>::Type;
	using TypeBase = SumMemberBase<TypeSum>;

	template<typename T>
	using TypeReference = TypeSum::template Reference<T>;

	using TypeHandle = TypeSum::Handle;


	enum class TypeKind
	{
		VOID,
		INT,
		FLOAT,
		STRUCT_OR_UNION,
		ENUM,
		TYPEDEF_NAME,
		POINTER,
		ARRAY,
		FUNCTION,
	};

	enum class TypePrecedence
	{
		LOWEST,
		POINTER,
		ARRAY,
		FUNCTION,
		// base type
		LEAF,
	};

	TypePrecedence precedence(const TypeBase& type)
	{
		using enum TypeKind;
		switch (type.ttype())
		{
			case VOID:            return TypePrecedence::LEAF;
			case INT:             return TypePrecedence::LEAF;
			case FLOAT:           return TypePrecedence::LEAF;
			case STRUCT_OR_UNION: return TypePrecedence::LEAF;
			case ENUM:            return TypePrecedence::LEAF;
			case TYPEDEF_NAME:    return TypePrecedence::LEAF;
			case POINTER:         return TypePrecedence::POINTER;
			case ARRAY:           return TypePrecedence::ARRAY;
			case FUNCTION:        return TypePrecedence::FUNCTION;
		}

		return TypePrecedence::LOWEST;
	}

	std::string format_type(const TypeBase& type, const std::string& name = "<anonymous>", TypePrecedence parent_predence = TypePrecedence::LOWEST)
	{
		using enum TypeKind;
		switch (type.ttype())
		{
			case VOID:
			{
				return "void";
			} break;

			case INT:
			{
				const auto& int_type = static_cast<const IntType&>(type);

				// <storage-class> <type-qualifier> <type-specifier> <type-width>
				return std::format("{} {} {} {}",
					_storage_class_map.at(int_type.storage),
					format_type_qualifier(int_type.qualifier),
					_type_signedness_map.at(int_type.signedness),
					_int_kind_map.at(int_type.int_kind));
			} break;

			case FLOAT:
			{
				const auto& float_type = static_cast<const FloatType&>(type);

				// <storage-class> <type-qualifier> <type-width>
				return std::format("{} {} {}",
					_storage_class_map.at(float_type.storage),
					format_type_qualifier(float_type.qualifier),
					_float_width_map.at(float_type.float_type));
			} break;

			case STRUCT_OR_UNION:
			{
				const auto& struct_or_union_type = static_cast<const StructOrUnionType&>(type);

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
					_storage_class_map.at(struct_or_union_type.storage),
					format_type_qualifier(struct_or_union_type.qualifier),
					_struct_or_union_type_map.at(struct_or_union_type.struct_or_union_kind),
					members);
			} break;

			case ENUM:
			{
				const auto& enum_type = static_cast<const EnumType&>(type);

				// <storage-class> <type-qualifier> enum tag 
				// NOTE: enumerators not shown since they don't fundamentally affect the type
				return std::format("{} {} enum {}", 
					_storage_class_map.at(enum_type.storage),
					format_type_qualifier(enum_type.qualifier),
					name);
			} break;

			case TYPEDEF_NAME:
			{
				const auto& typedef_name_type = static_cast<const TypedefName&>(type);

				// NOTE: just using the underyling alias name since the typedef doesn't affect the type's string representation
				return typedef_name_type.name;
			} break;

			case POINTER:
			{
				const auto& pointer_type = static_cast<const PointerType&>(type);
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

			case ARRAY:
			{
				const auto& array_type = static_cast<const ArrayType&>(type);
				const auto array_length = array_type.length.has_value() 
					? std::to_string(array_type.length.value()) 
					: "";

				// <element-type> [ <array-size> ]
				const auto name_string = std::format("{}[{}]", name, array_length);

				return format_type(array_type.element_type, name_string, TypePrecedence::ARRAY);
			} break;

			case FUNCTION:
			{
				const auto& function_type = static_cast<const FunctionType&>(type);

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

#endif
