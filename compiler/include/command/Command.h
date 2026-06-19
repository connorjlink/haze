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
		LabelCommand(std::string_view label, const Token& token)
			: CommandBase{ token }, label{ label }
		{
		}
	};
#define MAKE_LABEL_COMMAND(label, token) LabelCommand{ label, token }

	struct InstructionCommand : public CommandBase
	{
	public:
		ByteRange object_code;
		std::string_view branch_target;
		bool marked_for_deletion;
	
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
		InstructionCommand(const Token&, const ByteRange&, std::string_view = "");
	};
#define MAKE_INSTRUCTION_COMMAND(token, object_code, branch_target) InstructionCommand{ token, object_code, branch_target }

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
		DotOrgCommand(Address address, const Token& token)
			: CommandBase{ token }, address{ address }
		{
		}
	};
#define MAKE_DOTORG_COMMAND(address, token) DotOrgCommand{ address, token }

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
		DotByteCommand(const ByteRange& bytes, const Token& token)
			: CommandBase{ token }, bytes{ bytes }
		{
		}
	};
#define MAKE_DOTBYTE_COMMAND(bytes, token) DotbyteCommand{ bytes, token }


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
