import std;

#include <type/Type.h>
#include <type/Int.h>
#include <error/ErrorReporter.h>
#include <validator/TypeValidator.h>
#include <toolchain/CompilerParser.h>

// Haze TypeValidator.h
// (c) Connor J. Link. All Rights Reserved.

// `x` is the error context
// `v` is the expected value
// `e` is the variable against which to compare
#define VALIDATE_IMPL(x, v, e, a, b) if (e != v) \
{ \
	USE_UNSAFE(ErrorReporter)->post_error(parameters.context, parameters.filepath, \
		std::format(x " != {} (actually {})", a, b), NULL_TOKEN); \
	had_error = true; \
}

#define VALIDATE(x, v, e) VALIDATE_IMPL(x, v, e, v, e)

namespace hz
{
    TypeValidator::TypeValidator()
    {
        initialize_sum();

        // serialized primitive types to string
		const auto parameters0 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating primitive()");
		auto test0 = Test{ "primitive()", validate_primitive };
		test0.attach(parameters0);
		add_test(test0);

		// serialized pointer types to string
		const auto parameters1 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating pointer()");
		auto test1 = Test{ "pointer()", validate_pointer };
		test1.attach(parameters1);
		add_test(test1);

        // serialized enum types to string
        const auto parameters2 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating enum()");
        auto test2 = Test{ "enum()", validate_enum };
        test2.attach(parameters2);
        add_test(test2);

        // serialized struct types to string
        const auto parameters3 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating struct()");
        auto test3 = Test{ "struct()", validate_struct };
        test3.attach(parameters3);
        add_test(test3);

        // serialized union types to string
        const auto parameters4 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating union()");
        auto test4 = Test{ "union()", validate_union };
        test4.attach(parameters4);
        add_test(test4);

        // serialized function types to string
        const auto parameters5 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating function()");
        auto test5 = Test{ "function()", validate_function };
        test5.attach(parameters5);
        add_test(test5);

        // serialized array types to string
        const auto parameters6 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating array()");
        auto test6 = Test{ "array()", validate_array };
        test6.attach(parameters6);
        add_test(test6);

        // parsing primitive types from string
        const auto parameters7 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating parse_primitive()");
        auto test7 = Test{ "parse_primitive()", validate_parse_primitive };
        test7.attach(parameters7);
        add_test(test7);

        // parsing pointer types from string
        const auto parameters8 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating parse_pointer()");
        auto test8 = Test{ "parse_pointer()", validate_parse_pointer };
        test8.attach(parameters8);
        add_test(test8);

        // parsing enum types from string
        const auto parameters9 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating parse_enum()");
        auto test9 = Test{ "parse_enum()", validate_parse_enum };
        test9.attach(parameters9);
        add_test(test9);

        // parsing struct types from string
        const auto parameters10 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating parse_struct()");
        auto test10 = Test{ "parse_struct()", validate_parse_struct };
        test10.attach(parameters10);
        add_test(test10);

        // parsing union types from string
        const auto parameters11 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating parse_union()");
        auto test11 = Test{ "parse_union()", validate_parse_union };
        test11.attach(parameters11);
        add_test(test11);

        // parsing function types from string
        const auto parameters12 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating parse_function()");
        auto test12 = Test{ "parse_function()", validate_parse_function };
        test12.attach(parameters12);
        add_test(test12);

        // parsing array types from string
        const auto parameters13 = USE_SAFE(ErrorReporter)->open_context(__FILE__, "validating parse_array()");
        auto test13 = Test{ "parse_array()", validate_parse_array };
        test13.attach(parameters13);
        add_test(test13);
    }

    bool TypeValidator::validate_primitive(ErrorFrame parameters)
    {
        auto had_error = false;

        // int
        const auto case0 = Int{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .signedness = TypeSignedness::SIGNED, .int_type = IntWidth::INT };
        _int_handle = make_handle<Int, TypeSum::Storage>(sum.storage, case0);
        VALIDATE("int", format_type(case0), "int");

        // static const unsigned short
        const auto case1 = Int{ .storage = StorageClass::STATIC, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::UNSIGNED, .int_type = IntWidth::SHORT };
        _static_const_unsigned_short_handle = make_handle<Int, TypeSum::Storage>(sum.storage, case1);
        VALIDATE("static const unsigned short", format_type(case1), "static const unsigned short");
        
        // extern const volatile long long
        const auto case2 = Int{ .storage = StorageClass::EXTERN, .qualifier = TypeQualifier::CONST | TypeQualifier::VOLATILE, .signedness = TypeSignedness::SIGNED, .int_type = IntWidth::LONG_LONG };
        _extern_const_volatile_long_long_handle = make_handle<Int, TypeSum::Storage>(sum.storage, case2);
        VALIDATE("extern const volatile long long", format_type(case2), "extern const volatile long long");

        // const char
        const auto case3 = Int{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::CONST, .signedness = TypeSignedness::SIGNED, .int_type = IntWidth::CHAR };
        _const_char_handle = make_handle<Int, TypeSum::Storage>(sum.storage, case3);
        VALIDATE("const char", format_type(case3), "const char");

        return had_error;
    }

    bool TypeValidator::validate_pointer(ErrorFrame parameters)
    {
        auto had_error = false;

        // pointer to int
        const auto case0 = Pointer{ .pointee = _int_handle, .qualifier = TypeQualifier::NONE };
        VALIDATE("int*", format_type(case0), "int*");

        // const pointer to static const unsigned short
        const auto case1 = Pointer{ .pointee = _static_const_unsigned_short_handle, .qualifier = TypeQualifier::CONST };
        VALIDATE("static const unsigned short* const", format_type(case1), "static const unsigned short* const");

        // const volatile pointer to extern const volatile long long
        const auto case2 = Pointer{ .pointee = _extern_const_volatile_long_long_handle, .qualifier = TypeQualifier::CONST | TypeQualifier::VOLATILE };
        VALIDATE("extern const volatile long long* const volatile", format_type(case2), "extern const volatile long long* const volatile");

        return had_error;
    }

    bool TypeValidator::validate_enum(ErrorFrame parameters)
    {
        auto had_error = false;

        // const enum {}
        const auto case0 = Enum{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::CONST, .tag = "my_enum" };
        _const_enum = make_handle<Enum, TypeSum::Storage>(sum.storage, case0);
        VALIDATE("const enum my_enum", format_type(case0), "const enum my_enum");

        return had_error;
    }

    bool TypeValidator::validate_struct(ErrorFrame parameters)
    {
        auto had_error = false;

        // struct {}
        const auto case0 = StructOrUnion{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .tag = "my_struct" };
        _struct_no_members = make_handle<StructType, TypeSum::Storage>(sum.storage, case0);
        VALIDATE("struct my_struct", format_type(case0), "struct my_struct");

        // NOTE: this is not valid C due to static
        // extern const volatile struct { int member1; static const unsigned short member2; }
        const auto case1 = StructOrUnion{ .storage = StorageClass::EXTERN, .qualifier = TypeQualifier::CONST | TypeQualifier::VOLATILE, .struct_or_union_type = StructOrUnionType::STRUCT, .tag = "my_struct", .members = std::vector<StructOrUnionMember>{ { .name = "member1", .type = _int_handle }, { .name = "member2", .type = _static_const_unsigned_short_handle } } };
        _extern_const_volatile_struct_2_members = make_handle<StructType, TypeSum::Storage>(sum.storage, case1);
        VALIDATE("extern const volatile struct my_struct {\\n    int member1;\\n    static const unsigned short member2;\\n}", format_type(case1), "extern const volatile struct my_struct {\n    int member1;\n    static const unsigned short member2;\n}");

        return had_error;
    }

    bool TypeValidator::validate_union(ErrorFrame parameters)
    {
        auto had_error = false;

        // union {}
        const auto case0 = StructOrUnion{ .storage = StorageClass::AUTO, .qualifier = TypeQualifier::NONE, .tag = "my_union" };
        _union_no_members = make_handle<StructType, TypeSum::Storage>(sum.storage, case0);
        VALIDATE("union my_union {\\n}", format_type(case0), "union my_union {\n}");

        // NOTE: this is not valid C due to static
        // extern union { int member1; static const unsigned short member2; }
        const auto case1 = StructOrUnion{ .storage = StorageClass::EXTERN, .qualifier = TypeQualifier::NONE, .struct_or_union_type = StructOrUnionType::UNION, .tag = "my_union", .members = std::vector<StructOrUnionMember>{ { .name = "member1", .type = _int_handle }, { .name = "member2", .type = _static_const_unsigned_short_handle } } };
        _extern_union_2_members = make_handle<StructType, TypeSum::Storage>(sum.storage, case1);
        VALIDATE("extern union my_union {\\n    int member1;\\n    static const unsigned short member2;\\n}", format_type(case1), "extern union my_union {\n    int member1;\n    static const unsigned short member2;\n}");

        return had_error;
    }

    bool TypeValidator::validate_function(ErrorFrame parameters)
    {
        auto had_error = false;

        // NOTE: this is not valid C due to static
        // function (static const unsigned short) returning int
        const auto case0 = Function{ .return_type = _int_handle, .parameters = { _static_const_unsigned_short_handle }, .is_variadic = false };
        _int_pointer_function_long_double = make_handle<Function, TypeSum::Storage>(sum.storage, case0);
        VALIDATE("int(static const unsigned short)", format_type(case0), "int(static const unsigned short)");

        // pointer to function (pointer to const char, ...) returning const enum my_enum
        const auto case1_helper1 = Pointer{ .pointee = _const_char_handle, .qualifier = TypeQualifier::NONE };
        const auto case1_helper2 = Function{ .return_type = _const_enum, .parameters = { make_handle<Pointer, TypeSum::Storage>(sum.storage, case1_helper1) }, .is_variadic = true };
        const auto case1 = Pointer{ .pointee = make_handle<Function, TypeSum::Storage>(sum.storage, case1_helper2), .qualifier = TypeQualifier::CONST };
        _enum_function_pointer_const_char_pointer_variadic = make_handle<Pointer, TypeSum::Storage>(sum.storage, case1);
        VALIDATE("const enum my_enum(*)(const char*, ...)", format_type(case1), "const enum my_enum(*)(const char*, ...)");

        return had_error;
    }

    bool TypeValidator::validate_array(ErrorFrame parameters)
    {
        auto had_error = false;

        // array 10 of unsigned short
        const auto case0 = Array{ .element_type = _unsigned_short_array_10, .length = 10 };
        _unsigned_short_array_10 = make_handle<Array, TypeSum::Storage>(sum.storage, case0);
        VALIDATE("unsigned short[10]", format_type(case0), "unsigned short[10]");

        // array 5 of pointer to volatile int
        const auto case1_helper = Pointer{ .pointee = _int_handle, .qualifier = TypeQualifier::VOLATILE };
        const auto case1 = Array{ .element_type = make_handle<Pointer, TypeSum::Storage>(sum.storage, case1_helper), .length = 5 };
        _volatile_int_pointer_array_5 = make_handle<Array, TypeSum::Storage>(sum.storage, case1);
        VALIDATE("volatile int*[5]", format_type(case1), "volatile int*[5]");

        // pointer to array 5 of volatile int
        const auto case2_helper = Array{ .element_type = _int_handle, .length = 5 };
        const auto case2 = Pointer{ .pointee = make_handle<Array, TypeSum::Storage>(sum.storage, case2_helper), .qualifier = TypeQualifier::VOLATILE };
        _volatile_int_array_5_pointer = make_handle<Pointer, TypeSum::Storage>(sum.storage, case2);
        VALIDATE("volatile int(*)[5]", format_type(case2), "volatile int(*)[5]");

        // pointer to array 5 of pointer to function (pointer to const char) returning enum a
        const auto case3_helper1 = Array{ .element_type = _enum_function_pointer_const_char_pointer_variadic, .length = 5 };
        const auto case3_helper2 = Pointer{ .pointee = make_handle<Array, TypeSum::Storage>(sum.storage, case3_helper1), .qualifier = TypeQualifier::NONE };
        const auto case3 = Pointer{ .pointee = make_handle<Pointer, TypeSum::Storage>(sum.storage, case3_helper2), .qualifier = TypeQualifier::NONE };
        _pointer_to_array_5_enum_function_pointer_const_char_pointer_variadic = make_handle<Pointer, TypeSum::Storage>(sum.storage, case3);
        VALIDATE("const enum my_enum(*(*)[5])(const char*, ...)", format_type(case3), "const enum my_enum(*(*)[5])(const char*, ...)");

        return had_error;
    }

    bool TypeValidator::validate_parse_primitive(ErrorFrame parameters)
    {
        auto had_error = false;

        CompilerParser parser{ "int" };
        const auto type = parser.parse_type();
        VALIDATE("int", format_type(type), "int");

        CompilerParser parser2{ "static const unsigned short" };
        const auto type2 = parser2.parse_type();
        VALIDATE("static const unsigned short", format_type(type2), "static const unsigned short");

        CompilerParser parser3{ "extern const volatile long long" };
        const auto type3 = parser3.parse_type();
        VALIDATE("extern const volatile long long", format_type(type3), "extern const volatile long long");

        CompilerParser parser4{ "const char" };
        const auto type4 = parser4.parse_type();
        VALIDATE("const char", format_type(type4), "const char");

        return had_error;
    }

    bool TypeValidator::validate_parse_pointer(ErrorFrame parameters)
    {
        auto had_error = false;

        CompilerParser parser{ "int*" };
        const auto type = parser.parse_type();
        VALIDATE("int*", format_type(type), "int*");

        CompilerParser parser2{ "static const unsigned short* const" };
        const auto type2 = parser2.parse_type();
        VALIDATE("static const unsigned short* const", format_type(type2), "static const unsigned short* const");

        CompilerParser parser3{ "extern const volatile long long* const volatile" };
        const auto type3 = parser3.parse_type();
        VALIDATE("extern const volatile long long* const volatile", format_type(type3), "extern const volatile long long* const volatile");

        return had_error;
    }

    bool TypeValidator::validate_parse_enum(ErrorFrame parameters)
    {
        auto had_error = false;

        CompilerParser parser{ "const enum my_enum" };
        const auto type = parser.parse_type();
        VALIDATE("const enum my_enum", format_type(type), "const enum my_enum");

        return had_error;
    }

    bool TypeValidator::validate_parse_struct(ErrorFrame parameters)
    {
        auto had_error = false;

        CompilerParser parser{ "struct my_struct {}" };
        const auto type = parser.parse_type();
        VALIDATE("struct my_struct {\\n}", format_type(type), "struct my_struct {\n}");

        CompilerParser parser{ "struct my_struct { int member1; static const unsigned short member2; }" };
        const auto type2 = parser.parse_type();
        VALIDATE("struct my_struct {\\n    int member1;\\n    static const unsigned short member2;\\n}", format_type(type2), "struct my_struct {\n    int member1;\n    static const unsigned short member2;\n}");

        return had_error;
    }

    bool TypeValidator::validate_parse_union(ErrorFrame parameters)
    {
        auto had_error = false;

        CompilerParser parser{ "union my_union {}" };
        const auto type = parser.parse_type();
        VALIDATE("union my_union {\\n}", format_type(type), "union my_union {\n}");

        CompilerParser parser{ "union my_union { int member1; static const unsigned short member2; }" };
        const auto type2 = parser.parse_type();
        VALIDATE("union my_union {\\n    int member1;\\n    static const unsigned short member2;\\n}", format_type(type2), "union my_union {\n    int member1;\n    static const unsigned short member2;\n}");

        return had_error;
    }

    bool TypeValidator::validate_parse_function(ErrorFrame parameters)
    {
        auto had_error = false;

        CompilerParser parser{ "int(static const unsigned short)" };
        const auto type = parser.parse_type();
        VALIDATE("int(static const unsigned short)", format_type(type), "int(static const unsigned short)");

        CompilerParser parser2{ "const enum my_enum(*)(const char*, ...)" };
        const auto type2 = parser2.parse_type();
        VALIDATE("const enum my_enum(*)(const char*, ...)", format_type(type2), "const enum my_enum(*)(const char*, ...)");

        return had_error;
    }

    bool TypeValidator::validate_parse_array(ErrorFrame parameters)
    {
        auto had_error = false;

        CompilerParser parser{ "unsigned short[10]" };
        const auto type = parser.parse_type();
        VALIDATE("unsigned short[10]", format_type(type), "unsigned short[10]");

        CompilerParser parser2{ "volatile int*[5]" };
        const auto type2 = parser2.parse_type();
        VALIDATE("volatile int*[5]", format_type(type2), "volatile int*[5]");

        CompilerParser parser3{ "volatile int(*)[5]" };
        const auto type3 = parser3.parse_type();
        VALIDATE("volatile int(*)[5]", format_type(type3), "volatile int(*)[5]");

        CompilerParser parser4{ "const enum my_enum(*(*a)[5])(const char*, ...)" };
        const auto type4 = parser4.parse_type();
        VALIDATE("const enum my_enum(*(*)[5])(const char*, ...)", format_type(type4), "const enum my_enum(*(*)[5])(const char*, ...)");

        return had_error;
    }
}