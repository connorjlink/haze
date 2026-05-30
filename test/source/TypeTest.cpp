#include <cstddef>
#include <cstdlib>

#include <vector>
#include <string>
#include <string_view>
#include <format>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <optional>
#include <iostream>
#include <stack>
#include <queue>
#include <ranges>
#include <condition_variable>
#include <syncstream>
#include <filesystem>

#include <type/Type.h>
#include <type/IntType.h>
#include <toolchain/CompilerParser.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hz
{
	TEST_CLASS(TypeTest)
	{
	public:
		TEST_METHOD(ValidatePrimitive)
		{
			TypeSumStorage sum;

			// int
			const auto case0 = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::INT };
			Assert::AreEqual(std::string("int"), format_type(case0));

			// static const unsigned short
			const auto case1 = IntType{ .storage = StorageClass::STATIC, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::UNSIGNED, .int_kind = IntKind::SHORT };
			Assert::AreEqual(std::string("static const unsigned short"), format_type(case1));

			// extern const volatile long long
			const auto case2 = IntType{ .storage = StorageClass::EXTERN, .qualifier = TypeQualifier::CONST | TypeQualifier::VOLATILE, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::LONG_LONG };
			Assert::AreEqual(std::string("extern const volatile long long"), format_type(case2));

			// const char
			const auto case3 = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::CHAR };
			Assert::AreEqual(std::string("const char"), format_type(case3));
		}

		TEST_METHOD(ValidatePointer)
		{
			TypeSumStorage sum;

			// pointer to int
			const auto temp_int = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::INT };
			const auto int_handle = make_handle(sum, temp_int);
			const auto case0 = PointerType{ .pointee = int_handle, .qualifier = TypeQualifier::NONE };
			Assert::AreEqual(std::string("int*"), format_type(case0));

			// const pointer to static const unsigned short
			const auto temp_short = IntType{ .storage = StorageClass::STATIC, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::UNSIGNED, .int_kind = IntKind::SHORT };
			const auto static_const_unsigned_short_handle = make_handle(sum, temp_short);
			const auto case1 = PointerType{ .pointee = static_const_unsigned_short_handle, .qualifier = TypeQualifier::CONST };
			Assert::AreEqual(std::string("static const unsigned short* const"), format_type(case1));

			// const volatile pointer to extern const volatile long long
			const auto temp_ll = IntType{ .storage = StorageClass::EXTERN, .qualifier = TypeQualifier::CONST | TypeQualifier::VOLATILE, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::LONG_LONG };
			const auto extern_const_volatile_long_long_handle = make_handle(sum, temp_ll);
			const auto case2 = Pointer{ .pointee = extern_const_volatile_long_long_handle, .qualifier = TypeQualifier::CONST | TypeQualifier::VOLATILE };
			Assert::AreEqual(std::string("extern const volatile long long* const volatile"), format_type(case2));
		}

		TEST_METHOD(ValidateEnum)
		{
			TypeSumStorage sum;

			// const enum {}
			const auto case0 = EnumType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::CONST, .tag = "my_enum" };
			Assert::AreEqual(std::string("const enum my_enum"), format_type(case0));
		}

		TEST_METHOD(ValidateStruct)
		{
			TypeSumStorage sum;

			// int dependency
			const auto temp_int = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::INT };
			const auto int_handle = make_handle(sum, temp_int);

			// static const unsigned short dependency
			const auto temp_short = IntType{ .storage = StorageClass::STATIC, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::UNSIGNED, .int_kind = IntKind::SHORT };
			const auto static_const_unsigned_short_handle = make_handle(sum, temp_short);

			// struct {}
			const auto case0 = StructOrUnionType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .tag = "my_struct" };
			Assert::AreEqual(std::string("struct my_struct"), format_type(case0));

			const auto case1 = StructOrUnion{ .storage = StorageClass::EXTERN, .qualifier = TypeQualifier::CONST | TypeQualifier::VOLATILE, .struct_or_union_type = StructOrUnionType::STRUCT, .tag = "my_struct", .members = std::vector<StructOrUnionMember>{ {.name = "member1", .type = int_handle }, {.name = "member2", .type = static_const_unsigned_short_handle } } };
			Assert::AreEqual(std::string("extern const volatile struct my_struct {\n	int member1;\n	static const unsigned short member2;\n}"), format_type(case1));
		}

		TEST_METHOD(ValidateUnion)
		{
			TypeSumStorage sum;

			// int dependency
			const auto temp_int = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::INT };
			const auto int_handle = make_handle(sum, temp_int);

			// static const unsigned short dependency
			const auto temp_short = IntType{ .storage = StorageClass::STATIC, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::UNSIGNED, .int_kind = IntKind::SHORT };
			const auto static_const_unsigned_short_handle = make_handle(sum, temp_short);

			// union {}
			const auto case0 = StructOrUnion{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .tag = "my_union" };
			Assert::AreEqual(std::string("union my_union {\n}"), format_type(case0));

			const auto case1 = StructOrUnion{ .storage = StorageClass::EXTERN, .qualifier = TypeQualifier::NONE, .struct_or_union_type = StructOrUnionType::UNION, .tag = "my_union", .members = std::vector<StructOrUnionMember>{ {.name = "member1", .type = int_handle }, {.name = "member2", .type = static_const_unsigned_short_handle } } };
			Assert::AreEqual(std::string("extern union my_union {\n	int member1;\n	static const unsigned short member2;\n}"), format_type(case1));
		}

		TEST_METHOD(ValidateFunction)
		{
			TypeSumStorage sum;

			const auto temp_int = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::INT };
			const auto int_handle = make_handle(sum, temp_int);

			const auto temp_short = IntType{ .storage = StorageClass::STATIC, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::UNSIGNED, .int_kind = IntKind::SHORT };
			const auto static_const_unsigned_short_handle = make_handle(sum, temp_short);

			const auto temp_char = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::CHAR };
			const auto const_char_handle = make_handle(sum, temp_char);

			const auto temp_enum = EnumType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::CONST, .tag = "my_enum" };
			const auto const_enum = make_handle(sum, temp_enum);

			// function (static const unsigned short) returning int
			const auto case0 = Function{ .return_type = int_handle, .parameters = { static_const_unsigned_short_handle }, .is_variadic = false };
			Assert::AreEqual(std::string("int(static const unsigned short)"), format_type(case0));

			// pointer to function (pointer to const char, ...) returning const enum my_enum
			const auto case1_helper1 = Pointer{ .pointee = const_char_handle, .qualifier = TypeQualifier::NONE };
			const auto case1_helper2 = Function{ .return_type = const_enum, .parameters = { make_handle<Pointer, TypeSum::Storage>(sum.storage, case1_helper1) }, .is_variadic = true };
			const auto case1 = Pointer{ .pointee = make_handle<Function, TypeSum::Storage>(sum.storage, case1_helper2), .qualifier = TypeQualifier::CONST };
			Assert::AreEqual(std::string("const enum my_enum(*)(const char*, ...)"), format_type(case1));
		}

		TEST_METHOD(ValidateArray)
		{
			TypeSumStorage sum;

			// int dependency
			const auto temp_int = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::INT };
			const auto int_handle = make_handle(sum, temp_int);

			const auto temp_short = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .signedness = TypeSignedness::UNSIGNED, .int_kind = IntKind::SHORT };
			const auto unsigned_short_handle = make_handle(sum, temp_short);

			const auto temp_char = IntType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::SIGNED, .int_kind = IntKind::CHAR };
			const auto const_char_handle = make_handle(sum, temp_char);

			const auto temp_enum = EnumType{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::CONST, .tag = "my_enum" };
			const auto const_enum = make_handle(sum, temp_enum);

			// array 10 of unsigned short
			const auto case0 = Array{ .element_type = unsigned_short_handle, .length = 10 };
			const auto unsigned_short_array_10 = make_handle<Array, TypeSum::Storage>(sum.storage, case0);
			Assert::AreEqual(std::string("unsigned short[10]"), format_type(case0));

			// array 5 of pointer to volatile int
			const auto case1_helper = Pointer{ .pointee = int_handle, .qualifier = TypeQualifier::VOLATILE };
			const auto case1 = Array{ .element_type = make_handle<Pointer, TypeSum::Storage>(sum.storage, case1_helper), .length = 5 };
			Assert::AreEqual(std::string("volatile int*[5]"), format_type(case1));

			// pointer to array 5 of volatile int
			const auto case2_helper = Array{ .element_type = int_handle, .length = 5 };
			const auto case2 = Pointer{ .pointee = make_handle<Array, TypeSum::Storage>(sum.storage, case2_helper), .qualifier = TypeQualifier::VOLATILE };
			Assert::AreEqual(std::string("volatile int(*)[5]"), format_type(case2));

			// pointer to array 5 of pointer to function (pointer to const char) returning enum a
			const auto case3_func_param = Pointer{ .pointee = const_char_handle, .qualifier = TypeQualifier::NONE };
			const auto case3_func = Function{ .return_type = const_enum, .parameters = { make_handle<Pointer, TypeSum::Storage>(sum.storage, case3_func_param) }, .is_variadic = true };
			const auto case3_func_ptr = Pointer{ .pointee = make_handle<Function, TypeSum::Storage>(sum.storage, case3_func), .qualifier = TypeQualifier::NONE };
			const auto case3_helper1 = Array{ .element_type = make_handle<Pointer, TypeSum::Storage>(sum.storage, case3_func_ptr), .length = 5 };
			const auto case3_helper2 = Pointer{ .pointee = make_handle<Array, TypeSum::Storage>(sum.storage, case3_helper1), .qualifier = TypeQualifier::NONE };
			const auto case3 = Pointer{ .pointee = make_handle<Pointer, TypeSum::Storage>(sum.storage, case3_helper2), .qualifier = TypeQualifier::NONE };
			Assert::AreEqual(std::string("const enum my_enum(*(*)[5])(const char*, ...)"), format_type(case3));
		}

		TEST_METHOD(ValidateParsePrimitive)
		{
			CompilerParser parser{ "int" };
			Assert::AreEqual(std::string("int"), format_type(parser.parse_type()));

			CompilerParser parser2{ "static const unsigned short" };
			Assert::AreEqual(std::string("static const unsigned short"), format_type(parser2.parse_type()));

			CompilerParser parser3{ "extern const volatile long long" };
			Assert::AreEqual(std::string("extern const volatile long long"), format_type(parser3.parse_type()));

			CompilerParser parser4{ "const char" };
			Assert::AreEqual(std::string("const char"), format_type(parser4.parse_type()));
		}

		TEST_METHOD(ValidateParsePointer)
		{
			CompilerParser parser{ "int*" };
			Assert::AreEqual(std::string("int*"), format_type(parser.parse_type()));

			CompilerParser parser2{ "static const unsigned short* const" };
			Assert::AreEqual(std::string("static const unsigned short* const"), format_type(parser2.parse_type()));

			CompilerParser parser3{ "extern const volatile long long* const volatile" };
			Assert::AreEqual(std::string("extern const volatile long long* const volatile"), format_type(parser3.parse_type()));
		}

		TEST_METHOD(ValidateParseEnum)
		{
			CompilerParser parser{ "const enum my_enum" };
			Assert::AreEqual(std::string("const enum my_enum"), format_type(parser.parse_type()));
		}

		TEST_METHOD(ValidateParseStruct)
		{
			CompilerParser parser{ "struct my_struct {}" };
			Assert::AreEqual(std::string("struct my_struct {\n}"), format_type(parser.parse_type()));

			CompilerParser parser2{ "struct my_struct { int member1; static const unsigned short member2; }" };
			Assert::AreEqual(std::string("struct my_struct {\n	int member1;\n	static const unsigned short member2;\n}"), format_type(parser2.parse_type()));
		}

		TEST_METHOD(ValidateParseUnion)
		{
			CompilerParser parser{ "union my_union {}" };
			Assert::AreEqual(std::string("union my_union {\n}"), format_type(parser.parse_type()));

			CompilerParser parser2{ "union my_union { int member1; static const unsigned short member2; }" };
			Assert::AreEqual(std::string("union my_union {\n	int member1;\n	static const unsigned short member2;\n}"), format_type(parser2.parse_type()));
		}

		TEST_METHOD(ValidateParseFunction)
		{
			CompilerParser parser{ "int(static const unsigned short)" };
			Assert::AreEqual(std::string("int(static const unsigned short)"), format_type(parser.parse_type()));

			CompilerParser parser2{ "const enum my_enum(*)(const char*, ...)" };
			Assert::AreEqual(std::string("const enum my_enum(*)(const char*, ...)"), format_type(parser2.parse_type()));
		}

		TEST_METHOD(ValidateParseArray)
		{
			CompilerParser parser{ "unsigned short[10]" };
			Assert::AreEqual(std::string("unsigned short[10]"), format_type(parser.parse_type()));

			CompilerParser parser2{ "volatile int*[5]" };
			Assert::AreEqual(std::string("volatile int*[5]"), format_type(parser2.parse_type()));

			CompilerParser parser3{ "volatile int(*)[5]" };
			Assert::AreEqual(std::string("volatile int(*)[5]"), format_type(parser3.parse_type()));

			CompilerParser parser4{ "const enum my_enum(*(*a)[5])(const char*, ...)" };
			// Note: The previous validation ignored the variable name 'a' in formatting
			Assert::AreEqual(std::string("const enum my_enum(*(*)[5])(const char*, ...)"), format_type(parser4.parse_type()));
		}
	};
}
