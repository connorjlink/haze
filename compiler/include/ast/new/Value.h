#ifndef HAZE_VALUESUM_H
#define HAZE_VALUESUM_H

#include <ast/new/Sum.h>
#include <ast/new/Typing.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/Generator.h>
#include <utility/String.h>

// Haze ValueSum.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    // expose a strict polymorphic interface for values
    template<typename AnchorT>
    using ValueMethods = std::tuple
    <
        Method<&AnchorT::ttype, TagType()>,
        Method<&AnchorT::format, std::string()>,
        Method<&AnchorT::load_into, void(Generator&, RegisterIndex)>,
        Method<&AnchorT::store_from, void(Generator&, RegisterIndex)>,
    >;

    template<typename SumMemberT, typename SumStorageT>
    concept Value = SumTuple<SumMemberT, SumStorageT, ValueMethods<SumStorageT>>;


    class RegisterValue;
    class StackValue;
    // global object in the data segment
    class StaticValue;

    using ValueTypes = SumTypeList
    <
        RegisterValue,
        StackValue,
        StaticValue
    >;

    using ValueSum = MakeSum<ValueMethods, ValueTypes>::Type;
    using ValueBase = SumMemberBase<ValueSum>;


    class RegisterValue : public ValueBase
    {
    public:
        using RegisterIndex = std::uint8_t;

    public:
        RegisterIndex index;

    public:
        std::string format() const
        {
            return std::format("r{}", index);
        }

        void load_into(Generator& generator, RegisterIndex destination) const
        {
            if (destination != index)
            {
                generator.move(destination, index);
            }
        }

        void store_from(Generator& generator, RegisterIndex source) const
        {
            if (source != index)
            {
                generator.move(index, source);
            }
        }
    };

    class StackValue : public ValueBase
    {

    };

    class StaticValue 
        : public ValueBase
        , public InjectSingleton<ErrorReporter>
    {
    public:
        using MemoryIndex = std::uint32_t;

    private:
        

    public:
        MemoryIndex index;

    public:
        std::string format() const
        {
            static constexpr unsigned bits_per_hex_digit = std::countr_zero(16) + 1;
            static constexpr unsigned hex_digits = (sizeof(MemoryIndex) * CHAR_BIT + bits_per_hex_digit - 1) / bits_per_hex_digit;
            static constexpr const char* format_number = IntegerToStringLiteral<hex_digits>::value;
            static constexpr const char* left_half = "[{0:";
            static constexpr const char* right_half = "X}]";
            static constexpr auto left_format = compile_time_concat<compile_time_strlen(left_half), compile_time_strlen(format_number)>(left_half, format_number);
            static constexpr auto full_format = compile_time_concat<compile_time_strlen(left_format), compile_time_strlen(right_half)>(left_format, right_half);

            return std::format(full_format.data(), index);
        }

        void load_into(Generator& generator, RegisterIndex destination) const
        {
            generator.heap_read(destination, index.index);
        }

        void store_from(Generator& generator, RegisterIndex source) const
        {
            USE_SAFE(ErrorReporter)->post_uncorrectable("cannot write to static (read-only) data region", NULL_TOKEN);
        }
    };
}

#endif
