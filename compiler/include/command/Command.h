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
	// forward declare sum storage and self-referential types for facade

	struct CommandSumStorage;

	using CommandHandle = SumHandle<CommandSumStorage>;

	template<typename T>
	using CommandReference = SumReference<T, CommandSumStorage>;

	using CommandFacade = SumMemberBase<CommandSumStorage>;

	class CommandBase
		: public CommandFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		Address offset;
		Token token;

	public:
		CommandBase(const Token& token)
			: offset{}, token{ token }
		{
		}

	public:
		template<typename Self>
		CommandKind command_kind(this Self&& self)
		{
			switch (self.ttype())
			{
#define X(enumerator, type, name) case TypeIndexV<type, typename CommandSumStorage::Type>: return CommandKind::enumerator;
#include <command/defs/CommandKind.def>
#undef X
			}

			USE_SAFE(ErrorReporter)->post_error(std::format(
				"invalid command tag `{}`", self.ttype()), self.token);

			return CommandKind::LABEL;
		}
	};
}

namespace hz
{
	class LabelCommand : public CommandBase
	{
	private:
		std::string label;
		Address resolved_address;

	public:
		LabelCommand(const std::string& label, const Token& token)
			: CommandBase{ token }, label{ label }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		CommandHandle evaluate(const Storage&, Context&) const;
		CommandHandle optimize(const Storage&) const;
	};
#define MAKE_LABEL_COMMAND(label, token) LabelCommand{ label, token }

	class InstructionCommand : public CommandBase
	{
	public:
		ByteRange object_code;
		std::string branch_target;
		bool marked_for_deletion;

	public:
		InstructionCommand() = delete;
		InstructionCommand(const ByteRange&, const std::string& = "", const Token&);

	public:
		inline std::size_t length() const
		{
			return object_code.size();
		}


	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		CommandHandle evaluate(const Storage&, Context&) const;
		CommandHandle optimize(const Storage&) const;
	};
#define MAKE_INSTRUCTION_COMMAND(object_code, branch_target, token) InstructionCommand{ object_code, branch_target, token }

	class DotOrgCommand : public CommandBase
	{
	public:
		Address address;

	public:
		DotOrgCommand(Address address, const Token& token)
			: CommandBase{ token }, address{ address }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		CommandHandle evaluate(const Storage&, Context&) const;
		CommandHandle optimize(const Storage&) const;
	};
#define MAKE_DOTORG_COMMAND(address, token) DotOrgCommand{ address, token }

	class DotByteCommand : public CommandBase
	{
	private:
		ByteRange bytes;

	public:
		DotByteCommand(const ByteRange& bytes, const Token& token)
			: CommandBase{ token }, bytes{ bytes }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		CommandHandle evaluate(const Storage&, Context&) const;
		CommandHandle optimize(const Storage&) const;
	};
#define MAKE_DOTBYTE_COMMAND(bytes, token) DotbyteCommand{ bytes, token }


	using CommandTypes = SumTypeList
	<
		LabelCommand,
		InstructionCommand,
		DotOrgCommand,
		DotByteCommand,
	>;

	using CommandSum = MakeSum<ASTMethods, CommandTypes>::Type;

	template<typename T>
	using CommandReference = CommandSum::template Reference<T>;

	using CommandHandle = CommandSum::Handle;
}

#endif
