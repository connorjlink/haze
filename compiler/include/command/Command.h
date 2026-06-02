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

	FORWARD_DECLARE_SUM(Command)

	template<typename MethodsT>
	using CommandASTMethods = BaseASTMethods<MethodsT, CommandHandle>;

	DEFINE_SUM(Command, BASE_AST_METHODS)


	class CommandBase
		: public CommandFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using Storage = CommandSumStorage;

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
		CommandKind command_kind(this Self&&);
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


	template<typename SumMemberT, typename SumStorageT>
	concept CommandConcept = SumTuple<SumMemberT, SumStorageT, CommandASTMethods<SumStorageT>>;

	using CommandKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		COMMAND_KINDS(X)
#undef X
		void
	>;

	using CommandSumImplementation = MakeSum<CommandASTMethods, CommandKinds>::Type;

	struct CommandSumStorage : public CommandSumImplementation::Storage
	{
		using CommandSumImplementation::Storage::Storage;

		using Type = CommandSumImplementation::Type;
		using Anchor = CommandSumImplementation::Anchor;
	};


	template<typename Self>
	CommandKind CommandBase::command_kind(this Self&& self)
	{
		switch (self.tag_type())
		{
#define X(enumerator, type, name) case TypeIndexV<type, typename CommandSumStorage::Type>: return CommandKind::enumerator;
			COMMAND_KINDS(X)
#undef X
		}

		// error recovery does not care about command kind
		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid command tag `{}`", self.tag_type()), self.token);

		return CommandKind::LABEL;
	}
}

#endif
