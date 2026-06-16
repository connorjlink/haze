#ifndef HAZE_AST_H
#define HAZE_AST_H

#include <runtime/Evaluation.h>
#include <utility/Sum.h>
#include <utility/Typing.h>

// Haze AST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	FORWARD_DECLARE_SUM(Variable)

	using ASTTask = EvaluationTask<VariableHandle>;
	using ASTAwaiter = EvaluationAwaiter<VariableHandle>;

	struct ASTPromiseAwaiter
	{
	public:
		using PromiseType = ASTTask::promise_type;
		PromiseType* promise = nullptr;

	public:
		bool await_ready() const noexcept
		{
			return false;
		}

		bool await_suspend(std::coroutine_handle<PromiseType> handle) noexcept
		{
			promise = &handle.promise();
			return true;
		}

		PromiseType* await_resume() const noexcept
		{
			return promise;
		}
	};

	
	inline constexpr auto TAB = 4;
	inline constexpr auto MAX_INDENTATION = 40;
	
	namespace internal
	{
		constexpr auto space_buffer = []
		{
			auto array = std::array<char, MAX_INDENTATION + 1>{};

			for (auto i = 0uz; i < MAX_INDENTATION; i++)
			{
				array[i] = ' ';
			}
			array[MAX_INDENTATION] = '\0';

			return array;
		}();

		template<std::size_t... Is>
		constexpr auto make_indentation_table(std::index_sequence<Is...>)
		{
			return std::array<std::string_view, sizeof...(Is)>
			{
				std::string_view{ space_buffer.data() + (MAX_INDENTATION - Is), Is }...
			};
		}
	}

	constexpr auto indentation_table =
		internal::make_indentation_table(std::make_index_sequence<MAX_INDENTATION + 1>{});


#define BASE_AST_METHODS(X, handlet) \
	X(format,   std::string) \
	X(generate, void) \
	X(evaluate, ASTTask) \
	X(optimize, handlet)

	// expose a strict polymorphic interface for AST nodes, including external data like IR
	template<typename AnchorT, typename HandleT>
	using BaseASTMethods = AllButLastT
	<
#define X(name, handlet) METHOD_TUPLE_ENTRY(name, handlet)
		BASE_AST_METHODS(X, HandleT)
#undef X
		void
	>;


	template<typename SumMemberT, typename StorageT, typename MethodsT>
	concept IsASTNode = SumTuple<SumMemberT, StorageT, MethodsT>;
}

#endif