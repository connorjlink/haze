#include <cstddef>
#include <cstdlib>

#include <condition_variable>
#include <coroutine>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <ranges>
#include <stack>
#include <stacktrace>
#include <string>
#include <string_view>
#include <syncstream>
#include <unordered_map>
#include <variant>
#include <vector>

#include <toolchain/CompilerParser.h>
#include <toolchain/Lexer.h>
#include <type/IntType.h>
#include <type/Type.h>

// why in the world is this required?
#define NULL 0
#include "CppUnitTest.h"
#undef CONST

// Haze TypeTest.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;
	using namespace std::string_literals;

	TEST_CLASS(TypeTest)
	{
	public:
		TypeStorage type_storage;
		ExpressionStorage expression_storage;

	public:
		TEST_METHOD(ValidatePrimitive)
		{
			// int
			const auto case0 = IntType{ StorageClass::AUTO, TypeQualifier::NONE, TypeSignedness::SIGNED, IntKind::INT };
			Assert::AreEqual("int"s, to_string(case0));

			// static const unsigned short
			const auto case1 = IntType{ StorageClass::STATIC, TypeQualifier::CONST, TypeSignedness::UNSIGNED, IntKind::SHORT };
			Assert::AreEqual("static const unsigned short"s, to_string(case1));

			// extern const volatile long long
			const auto case2 = IntType{ StorageClass::EXTERN, TypeQualifier::CONST | TypeQualifier::VOLATILE, TypeSignedness::SIGNED, IntKind::LONG_LONG };
			Assert::AreEqual("extern const volatile long long"s, to_string(case2));

			// const char
			const auto case3 = IntType{ StorageClass::AUTO, TypeQualifier::CONST, TypeSignedness::SIGNED, IntKind::CHAR };
			Assert::AreEqual("const char"s, to_string(case3));
		}

		TEST_METHOD(ValidatePointer)
		{
			// pointer to int
			auto int_type = IntType{ StorageClass::AUTO, TypeQualifier::NONE, TypeSignedness::SIGNED, IntKind::INT };
			const auto int_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(int_type));
			const auto case0 = PointerType{ int_handle, TypeQualifier::NONE };
			Assert::AreEqual("int*"s, to_string(case0));

			// const pointer to static const unsigned short
			auto temp_short = IntType{ StorageClass::STATIC, TypeQualifier::CONST, TypeSignedness::UNSIGNED, IntKind::SHORT };
			const auto static_const_unsigned_short_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_short));
			const auto case1 = PointerType{ static_const_unsigned_short_handle, TypeQualifier::CONST };
			Assert::AreEqual("static const unsigned short* const"s, to_string(case1));

			// const volatile pointer to extern const volatile long long
			auto temp_ll = IntType{ StorageClass::EXTERN, TypeQualifier::CONST | TypeQualifier::VOLATILE, TypeSignedness::SIGNED, IntKind::LONG_LONG };
			const auto extern_const_volatile_long_long_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_ll));
			const auto case2 = PointerType{ extern_const_volatile_long_long_handle, TypeQualifier::CONST | TypeQualifier::VOLATILE };
			Assert::AreEqual("extern const volatile long long* const volatile"s, to_string(case2));
		}

		TEST_METHOD(ValidateEnum)
		{
			// const enum {}
			const auto case0 = EnumType{ StorageClass::AUTO, TypeQualifier::CONST, std::nullopt, "my_enum" };
			Assert::AreEqual("const enum my_enum"s, to_string(case0));
		}

		TEST_METHOD(ValidateStruct)
		{
			// int dependency
			auto temp_int = IntType{ StorageClass::AUTO, TypeQualifier::NONE, TypeSignedness::SIGNED, IntKind::INT };
			const auto int_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_int));

			// static const unsigned short dependency
			auto temp_short = IntType{ StorageClass::STATIC, TypeQualifier::CONST, TypeSignedness::UNSIGNED, IntKind::SHORT };
			const auto static_const_unsigned_short_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_short));

			// struct {}
			const auto case0 = StructOrUnionType{ StorageClass::AUTO, TypeQualifier::NONE, StructOrUnionKind::STRUCT, std::nullopt, "my_struct" };
			Assert::AreEqual("struct my_struct"s, to_string(case0));

			const auto case1 = StructOrUnionType{ StorageClass::EXTERN, TypeQualifier::CONST | TypeQualifier::VOLATILE, StructOrUnionKind::STRUCT, std::vector<StructOrUnionType::Member>{ {.name = "member1", .type = int_handle }, {.name = "member2", .type = static_const_unsigned_short_handle } }, "my_struct" };
			Assert::AreEqual("extern const volatile struct my_struct {\n	int member1;\n	static const unsigned short member2;\n}"s, to_string(case1));
		}

		TEST_METHOD(ValidateUnion)
		{
			// int dependency
			auto temp_int = IntType{ StorageClass::AUTO, TypeQualifier::NONE, TypeSignedness::SIGNED, IntKind::INT };
			const auto int_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_int));

			// static const unsigned short dependency
			auto temp_short = IntType{ StorageClass::STATIC, TypeQualifier::CONST, TypeSignedness::UNSIGNED, IntKind::SHORT };
			const auto static_const_unsigned_short_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_short));

			// union {}
			const auto case0 = StructOrUnionType{ StorageClass::AUTO, TypeQualifier::NONE, StructOrUnionKind::UNION, std::nullopt, "my_union" };
			Assert::AreEqual("union my_union {\n}"s, to_string(case0));

			const auto case1 = StructOrUnionType{ StorageClass::EXTERN, TypeQualifier::NONE, StructOrUnionKind::UNION, std::vector<StructOrUnionType::Member>{ {.name = "member1", .type = int_handle }, {.name = "member2", .type = static_const_unsigned_short_handle } }, "my_union" };
			Assert::AreEqual("extern union my_union {\n	int member1;\n	static const unsigned short member2;\n}"s, to_string(case1));
		}

		TEST_METHOD(ValidateFunction)
		{
			auto temp_int = IntType{ StorageClass::AUTO, TypeQualifier::NONE, TypeSignedness::SIGNED, IntKind::INT };
			const auto int_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_int));

			auto temp_short = IntType{ StorageClass::STATIC, TypeQualifier::CONST, TypeSignedness::UNSIGNED, IntKind::SHORT };
			const auto static_const_unsigned_short_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_short));

			auto temp_char = IntType{ StorageClass::AUTO, TypeQualifier::CONST, TypeSignedness::SIGNED, IntKind::CHAR };
			const auto const_char_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_char));

			auto temp_enum = EnumType{ StorageClass::AUTO, TypeQualifier::CONST, std::nullopt, "my_enum" };
			const auto const_enum = MAKE_HANDLE(EnumType, Type, type_storage, std::move(temp_enum));

			// function (static const unsigned short) returning int
			const auto case0 = FunctionType{ { static_const_unsigned_short_handle }, int_handle, false };
			Assert::AreEqual("int(static const unsigned short)"s, to_string(case0));

			// pointer to function (pointer to const char, ...) returning const enum my_enum
			auto case1_helper1 = PointerType{ const_char_handle, TypeQualifier::NONE };
			auto case1_helper2 = FunctionType{ { MAKE_HANDLE(PointerType, Type, type_storage, std::move(case1_helper1)) }, const_enum, true };
			const auto case1 = PointerType{ MAKE_HANDLE(FunctionType, Type, type_storage, std::move(case1_helper2)), TypeQualifier::CONST };
			Assert::AreEqual("const enum my_enum(*)(const char*, ...)"s, to_string(case1));
		}

		TEST_METHOD(ValidateArray)
		{
			// int dependency
			auto temp_int = IntType{ StorageClass::AUTO, TypeQualifier::NONE, TypeSignedness::SIGNED, IntKind::INT };
			const auto int_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_int));

			auto temp_short = IntType{ StorageClass::AUTO, TypeQualifier::NONE, TypeSignedness::UNSIGNED, IntKind::SHORT };
			const auto unsigned_short_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_short));

			auto temp_char = IntType{ StorageClass::AUTO, TypeQualifier::CONST, TypeSignedness::SIGNED, IntKind::CHAR };
			const auto const_char_handle = MAKE_HANDLE(IntType, Type, type_storage, std::move(temp_char));

			auto temp_enum = EnumType{ StorageClass::AUTO, TypeQualifier::CONST, std::nullopt, "my_enum" };
			const auto const_enum = MAKE_HANDLE(EnumType, Type, type_storage, std::move(temp_enum));

			// array 10 of unsigned short
			auto case0 = ArrayType{ unsigned_short_handle, 10 };
			const auto unsigned_short_array_10 = MAKE_HANDLE(ArrayType, Type, type_storage, std::move(case0));
			Assert::AreEqual("unsigned short[10]"s, to_string(case0));

			// array 5 of pointer to volatile int
			auto case1_helper = PointerType{ int_handle, TypeQualifier::VOLATILE };
			const auto case1 = ArrayType{ MAKE_HANDLE(PointerType, Type, type_storage, std::move(case1_helper)), 5 };
			Assert::AreEqual("volatile int*[5]"s, to_string(case1));

			// pointer to array 5 of volatile int
			auto case2_helper = ArrayType{ int_handle, 5 };
			const auto case2 = PointerType{ MAKE_HANDLE(ArrayType, Type, type_storage, std::move(case2_helper)), TypeQualifier::VOLATILE };
			Assert::AreEqual("volatile int(*)[5]"s, to_string(case2));

#pragma message("TODO: add per-type_storage handle/reference generation logic to reduce the wordiness by self-specializing the make_XX templates")

			// pointer to array 5 of pointer to function (pointer to const char) returning enum a
			auto case3_func_param = PointerType{ const_char_handle, TypeQualifier::NONE };
			auto case3_func = FunctionType{ { MAKE_HANDLE(PointerType, Type, type_storage, std::move(case3_func_param)) }, const_enum, true };
			auto case3_func_ptr = PointerType{ MAKE_HANDLE(FunctionType, Type, type_storage, std::move(case3_func)), TypeQualifier::NONE };
			auto case3_helper1 = ArrayType{ MAKE_HANDLE(PointerType, Type, type_storage, std::move(case3_func_ptr)), 5 };
			auto case3_helper2 = PointerType{ MAKE_HANDLE(ArrayType, Type, type_storage, std::move(case3_helper1)), TypeQualifier::NONE };
			const auto case3 = PointerType{ MAKE_HANDLE(PointerType, Type, type_storage, std::move(case3_helper2)), TypeQualifier::NONE };
			Assert::AreEqual("const enum my_enum(*(*)[5])(const char*, ...)"s, to_string(case3));
		}

#define TEST_CASE_HELPER(typestring, counter) \
		static const auto test##counter = #typestring##s; \
		Lexer lexer##counter{ #typestring##s }; \
		const auto tokens##counter = lexer##counter.lex(); \
		CompilerParser parser##counter{ std::move(tokens##counter), expression_storage }; \
		Assert::AreEqual(test##counter, to_string(parser##counter.parse_type())) /* explicit semicolon omission */
#define TEST_CASE_INDIRECT(typestring, counter) TEST_CASE_HELPER(typestring, counter) /* C macro subsitution precedence rules :( */
#define TEST_CASE(typestring) TEST_CASE_INDIRECT(typestring, __COUNTER__)

		TEST_METHOD(ValidateParsePrimitive)
		{
			TEST_CASE(int);
			TEST_CASE(static const unsigned short);
			TEST_CASE(extern const volatile long long);
			TEST_CASE(const char);
		}

		TEST_METHOD(ValidateParsePointer)
		{
			TEST_CASE(int*);
			TEST_CASE(static const unsigned short* const);
			TEST_CASE(extern const volatile long long* const volatile);
		}

		TEST_METHOD(ValidateParseEnum)
		{
			TEST_CASE(const enum my_enum);
		}

		TEST_METHOD(ValidateParseStruct)
		{
			TEST_CASE(struct my_struct {});
			TEST_CASE(struct my_struct { int member1; static const unsigned short member2; });
		}

		TEST_METHOD(ValidateParseUnion)
		{
			TEST_CASE(union my_union {});
			TEST_CASE(union my_union { int member1; static const unsigned short member2; });
			TEST_CASE(union my_union { int member1; struct { unsigned char byte1; unsigned char byte2; signed short bytes34; } member2; });
		}

		TEST_METHOD(ValidateParseFunction)
		{
			TEST_CASE(int(static const unsigned short));
			TEST_CASE(const enum my_enum(*)(const char*, ...));
		}

		TEST_METHOD(ValidateParseArray)
		{
			TEST_CASE(unsigned short[10]);
			TEST_CASE(volatile int*[5]);
			TEST_CASE(volatile int(*)[5]);
			TEST_CASE(const enum my_enum(*(*a)[5])(const char*, ...));
		}

#undef TEST_CASE
#undef TEST_CASE_INDIRECT
#undef TEST_CASE_HELPER
	};
}
