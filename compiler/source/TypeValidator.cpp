import std;

#include <type/Type.h>
#include <type/Int.h>
#include <error/ErrorReporter.h>
#include <validator/TypeValidator.h>

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



        // TODO: fill in all of the test cases
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