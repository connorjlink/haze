#ifndef HAZE_TYPE_H
#define HAZE_TYPE_H

#include <data/DependencyInjector.h>
#include <symbol/SymbolDatabase.h>
#include <type/Void.h>
#include <type/Int.h>
#include <type/Float.h>
#include <type/Pointer.h>
#include <type/Float.h>
#include <type/StructOrUnion.h>
#include <type/Enum.h>
#include <type/TypedefName.h>
#include <type/Array.h>
#include <type/Pointer.h>
#include <type/Function.h>
#include <ast/new/Sum.h>

// Haze Type.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// expose a strict polymorphic interface for types
	template<typename AnchorT>
	using TypeMethods = std::tuple
	<
		Method<&AnchorT::ttype, TypeType()>,
		Method<&AnchorT::size, Offset()>,
		Method<&AnchorT::is_complete, bool()>,
	>;

	template<typename SumMemberT, typename SumStorageT>
	concept Type = SumTuple<SumMemberT, SumStorageT, TypeMethods<SumStorageT>>;

	class Void;
	class Int;
	class Float;
	class StructOrUnion;
	class Enum;
	class TypedefName;
	class Pointer;
	class Array;
	class Function;

	using TypeTypes = SumTypeList
	<
		Void,
		Int,
		Float,
		StructOrUnion,
		Enum,
		TypedefName,
		Pointer,
		Array,
		Function,
	>;

	using TypeSum = MakeSum<TypeMethods, TypeTypes>::Type;
	using TypeBase = SumMemberBase<TypeSum>;

	enum class TypeType
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

	enum class Precedence
	{
		LOWEST,
		POINTER,	
		ARRAY,
		FUNCTION,
		LEAF, // base type	
	};

	Precedence precedence(const TypeBase& type)
	{
		using enum TypeType;
		switch (type.ttype())
		{
			case VOID:            return Precedence::LEAF;
			case INT:             return Precedence::LEAF;
			case FLOAT:           return Precedence::LEAF;
			case STRUCT_OR_UNION: return Precedence::LEAF;
			case ENUM:            return Precedence::LEAF;
			case TYPEDEF_NAME:    return Precedence::LEAF;
			case POINTER:         return Precedence::POINTER;
			case ARRAY:           return Precedence::ARRAY;
			case FUNCTION:        return Precedence::FUNCTION;
		}

		return Precedence::LOWEST;
	}

	std::string format_type(const TypeBase& type, const std::string& name = "", Precedence parent_predence = Precedence::LOWEST)
	{
		using enum TypeType;
		switch (type.ttype())
		{
			case VOID:
			{
				return "void";
			} break;

			case INT:
			{
				const auto& int_type = static_cast<const Int&>(type);

				// <storage-class> <type-qualifier> <type-specifier> <type-width>
				return std::format("{} {} {} {}",
					_type_storage_map.at(int_type.storage),
					format_type_qualifier(int_type.qualifier),
					_type_signedness_map.at(int_type.signedness),
					_int_width_map.at(int_type.int_type));
			} break;

			case FLOAT:
			{
				const auto& float_type = static_cast<const Float&>(type);

				// <storage-class> <type-qualifier> <type-width>
				return std::format("{} {} {}",
					_type_storage_map.at(float_type.storage),
					format_type_qualifier(float_type.qualifier),
					_float_width_map.at(float_type.float_type));
			} break;

			case STRUCT_OR_UNION:
			{
				const auto& struct_or_union_type = static_cast<const StructOrUnion&>(type);

				if (!struct_or_union_type.members.has_value())
				{
					CommonErrors::invalid_struct_or_union_type(struct_or_union_type.struct_or_union_type, NULL_TOKEN);
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
					_type_storage_map.at(struct_or_union_type.storage),
					format_type_qualifier(struct_or_union_type.qualifier),
					_struct_or_union_type_map.at(struct_or_union_type.struct_or_union_type),
					members);
			} break;

			case ENUM:
			{
				const auto& enum_type = static_cast<const Enum&>(type);

				// <storage-class> <type-qualifier> enum tag 
				// NOTE: enumerators not shown since they don't fundamentally affect the type
				return std::format("{} {} enum {}", 
					_type_storage_map.at(enum_type.storage),
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
				const auto& pointer_type = static_cast<const Pointer&>(type);
				const auto qualifier_string = format_type_qualifier(pointer_type.qualifier);

				// <pointee-type>* <pointer-qualifiers>
				auto pointer_string = std::format("*{}{}{}",
					(qualifier_string.empty() ? "" : " "),
					qualifier_string,
					name);

				if (parent_predence > Precedence::POINTER && parent_predence != Precedence::LEAF)
				{
					pointer_string = std::format("({})", pointer_string);
				}

				return format_type(pointer_type.pointee, pointer_string, Precedence::POINTER);
			} break;

			case ARRAY:
			{
				const auto& array_type = static_cast<const Array&>(type);
				const auto array_length = array_type.length.has_value() 
					? std::to_string(array_type.length.value()) 
					: "";

				// <element-type> [ <array-size> ]
				const auto name_string = std::format("{}[{}]", name, array_length);

				return format_type(array_type.element_type, name_string, Precedence::ARRAY);
			} break;

			case FUNCTION:
			{
				const auto& function_type = static_cast<const Function&>(type);

				auto parameters = function_type.parameters
					| std::views::transform([](const auto& parameter) 
					{ 
						return format_type(parameter, "", Precedence::FUNCTION); 
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
				return format_type(function_type.return_type, name_string, Precedence::FUNCTION);
			}
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(
			"invalid type for string formatting", NULL_TOKEN);
		return "";
	}
}

#endif
