#ifndef HAZE_VALUESUM_H
#define HAZE_VALUESUM_H

#include <ast/new/Sum.h>
#include <ast/new/Typing.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/Generator.h>
#include <cli/ArchitectureType.h>
#include <utility/Constants.h>
#include <utility/Strings.h>

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
        Method<&AnchorT::load_into, void(Generator&, Register)>,
        Method<&AnchorT::store_from, void(Generator&, Register)>,
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
        Register index;

    public:
        std::string format() const
        {
            return std::format("r{}", index);
        }

        void load_into(Generator& generator, Register destination) const
        {
            if (destination != index)
            {
                generator.move(destination, index);
            }
        }

        void store_from(Generator& generator, Register source) const
        {
            if (source != index)
            {
                generator.move(index, source);
            }
        }
    };

    class StackValue 
        : public ValueBase
        , public InjectSingleton<CommandLineOptions>
    {
    public:
        // relative offset from the base pointer, negative is below the current stack frame
        Offset index;

    public:
        std::string format() const
        {
            const auto architecture_type = USE_SAFE(CommandLineOptions)->architecture;
            const auto format = get_stack_frame_pointer(architecture_type);

            return std::format("[{} + {}]", format, index);
        }

        void load_into(Generator& generator, Register destination) const
        {
            generator.stack_read(destination, index);
        }

        void store_from(Generator& generator, Register source) const
        {
            generator.stack_write(index, source);
        }
    };

    class StaticValue 
        : public ValueBase
        , public InjectSingleton<ErrorReporter>
    {
    public:
        Address index;

    public:
        std::string format() const
        {
            static constexpr unsigned bits_per_hex_digit = std::countr_zero(16);
            static constexpr unsigned hex_digits = (sizeof(Address) * CHAR_BIT + bits_per_hex_digit - 1) / bits_per_hex_digit;

            // NOTE: +2 required to account for the `0x` prefix
            return std::format("[{:#0{}x}]", index, hex_digits + 2);
        }

        void load_into(Generator& generator, Register destination) const
        {
            generator.heap_read(destination, index.index);
        }

        void store_from(Generator& generator, Register source) const
        {
            USE_SAFE(ErrorReporter)->post_uncorrectable("cannot write to static (read-only) data region", NULL_TOKEN);
        }
    };
}

#endif
