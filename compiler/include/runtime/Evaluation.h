#ifndef HAZE_EVALUATION_H
#define HAZE_EVALUATION_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/Variable.h>
#include <utility/Exception.h>

// Haze Evaluation.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct ControlFlowKind
	{
		NONE,
		RETURN,
		BREAK,
		CONTINUE,
	};

	namespace internal
	{
		template<typename T>
		struct InjectValue
		{
		public:
			T value;

		public:
			void return_value(T value)
			{
				this->value = value;
			}
		};

		template<typename>
		struct InjectVoid
		{
		public:
			void return_void()
			{
				// intrinsic
			}
		};

		template<typename T, typename InjectT>
		struct EvaluationTask
		{
		public:
			struct promise_type : public InjectT
			{
			public:
				ControlFlowKind kind = ControlFlowKind::NONE;

			public:
				EvaluationTask get_return_object()
				{
					return { std::coroutine_handle<promise_type>::from_promise(*this) };
				}

				std::suspend_always initial_suspend() noexcept
				{
					return {};
				}

				std::suspend_always final_suspend() noexcept
				{
					return {};
				}

				void unhandled_exception()
				{
					USE_UNSAFE(ErrorReporter)->post_error(
						"unhandled exception during evaluation", NULL_TOKEN);
					throw EvaluationFailureException(std::stacktrace::current());
				}
			};

		public:
			std::coroutine_handle<promise_type> handle;

		public:
			EvaluationTask(std::coroutine_handle<promise_type> handle)
				: handle{ handle }
			{
			}

			~EvaluationTask()
			{
				if (handle)
				{
					handle.destroy();
				}
			}

			// noncopyable
			EvaluationTask(const EvaluationTask&) = delete;
			EvaluationTask& operator=(const EvaluationTask&) = delete;

			EvaluationTask(EvaluationTask&& other) noexcept
				: handle{ std::exchange(other.handle, nullptr) }
			{
			}

			EvaluationTask& operator=(EvaluationTask&& other) noexcept
			{
				if (this != &other)
				{
					if (handle)
					{
						handle.destroy();
					}
					handle = std::exchange(other.handle, nullptr);
				}
				return *this;
			}
		};

		template<typename T>
		using InjectedTag = std::conditional_t<std::is_void_v<T>, InjectVoid<T>, InjectValue<T>>;
	}

	template<typename T>
	using EvaluationTask = internal::EvaluationTask<T, internal::InjectedTag<T>>;


	template<typename T = void>
	struct EvaluationAwaiter
	{
	public:
		EvaluationTask<T> task;

	public:
		bool await_ready() noexcept
		{
			return task.handle ? task.handle.done() : true;
		}

		std::coroutine_handle<> await_suspend(std::coroutine_handle<> handle) noexcept
		{
			return task.handle;
		}

		T await_resume() noexcept
		{
			if constexpr (std::is_void_v<T>)
			{
				return;
			}

			return task.handle.promise().value;
		}

	public:
		EvaluationAwaiter(EvaluationTask<T> task)
			: task{ std::move(task) }
		{
		}
	};

}

#endif
