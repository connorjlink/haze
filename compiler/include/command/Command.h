#ifndef HAZE_COMMAND_H
#define HAZE_COMMAND_H

#include <ast/AST.h>
#include <command/defs/CommandKind.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/Constants.h>
#include <utility/Typing.h>
#include <utility/Sum.h>

// Haze Command.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Context;

	FORWARD_DECLARE_SUM(Command)

#define COMMAND_AST_METHODS(X, handlet) \
	BASE_AST_METHODS(X, handlet) \
	X(command_kind, CommandKind)

	DEFINE_SUM(Command, COMMAND_AST_METHODS)


	struct CommandBase
		: public CommandFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using Storage = CommandStorage;

	public:
		Address offset;
		Token token;

	public:
		CommandBase(const Token& token)
			: offset{}, token{ token }
		{
		}
	};
}

namespace hz
{
	struct LabelCommand : public CommandBase
	{
	private:
		std::string_view label;
		Address resolved_address;

	public:
		CommandKind command_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		CommandHandle evaluate(const Storage&, Context&) const;
		CommandHandle optimize(const Storage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		LabelCommand(const Token& token, std::string_view label)
			: CommandBase{ token }, label{ label }
		{
		}
	};
#define MAKE_LABEL_COMMAND(token, label) MAKE_REFERENCE(LabelCommand, Command, EXPAND(LabelCommand{ token, label }), command_storage)

	struct InstructionCommand : public CommandBase
	{
	public:
		ByteRange object_code;
		std::string_view branch_target;
		bool marked_for_deletion = false;
	
	public:
		inline std::size_t length() const
		{
			return object_code.size();
		}

	public:
		CommandKind command_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		CommandHandle evaluate(const Storage&, Context&) const;
		CommandHandle optimize(const Storage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		InstructionCommand() = delete;
		InstructionCommand(const Token& token, ByteRange object_code, std::string_view branch_target = "")
			: CommandBase{ token }, object_code{ std::move(object_code) }, branch_target{ branch_target }
		{
		}
	};
#define MAKE_INSTRUCTION_COMMAND(token, object_code, branch_target) MAKE_REFERENCE(InstructionCommand, Command, EXPAND(InstructionCommand{ token, object_code, branch_target }), command_storage)

	struct DotOrgCommand : public CommandBase
	{
	public:
		Address address;

	public:
		CommandKind command_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		CommandHandle evaluate(const Storage&, Context&) const;
		CommandHandle optimize(const Storage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		DotOrgCommand(const Token& token, Address address)
			: CommandBase{ token }, address{ address }
		{
		}
	};
#define MAKE_DOTORG_COMMAND(token, address) MAKE_REFERENCE(DotOrgCommand, Command, EXPAND(DotOrgCommand{ token, address }), command_storage)

	struct DotByteCommand : public CommandBase
	{
	private:
		ByteRange bytes;

	public:
		CommandKind command_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		CommandHandle evaluate(const Storage&, Context&) const;
		CommandHandle optimize(const Storage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		DotByteCommand(const Token& token, ByteRange bytes)
			: CommandBase{ token }, bytes{ std::move(bytes) }
		{
		}
	};
#define MAKE_DOTBYTE_COMMAND(token, bytes) MAKE_REFERENCE(DotByteCommand, Command, EXPAND(DotByteCommand{ token, bytes }), command_storage)


	template<typename SumMemberT, typename StorageT>
	concept IsCommand = SumTuple<SumMemberT, StorageT, CommandMethods<typename StorageT::Anchor>>;

	using CommandKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		COMMAND_KINDS(X)
#undef X
		void
	>;

	using CommandSumImplementation = MakeSum<CommandMethods, CommandKinds>::Type;

	struct CommandStorage : public CommandSumImplementation::Storage
	{
		using CommandSumImplementation::Storage::Storage;

		using Type = CommandSumImplementation::Type;
		using Anchor = CommandSumImplementation::Anchor;
	};
}

#endif
