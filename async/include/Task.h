#ifndef HAZE_TASK_H
#define HAZE_TASK_H

// Haze Task.h
// (c) Connor J. Link. All Rights Reserved.

static_assert(_MSC_VER, "This code must be compiled with MSVC to use its native Windows-exclusive std::async threadpool dispatcher.");

namespace hz
{
	enum class ContinuationPolicy
	{
		ReturnToCaller,
		RunAnywhere
	};

	template<typename T = void>
	class Task
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

	private:
		handle_type coroutine;

	public:
		Task& operator=(Task&& t) noexcept
		{
			if (this != &t)
			{
				if (coroutine)
				{
					coroutine.destroy();
				}
				coroutine = std::exchange(t.coroutine, nullptr);
			}
			return *this;
		}

		bool await_ready() const noexcept { return false; }

		// Awaiter with continuation control
		auto await_suspend(std::coroutine_handle<> awaiting, ContinuationPolicy policy = ContinuationPolicy::ReturnToCaller)
		{
			coroutine.promise().continuation = awaiting;
			if (policy == ContinuationPolicy::RunAnywhere)
			{
				std::async(std::launch::async, [coroutine = this->coroutine]() mutable
				{
					coroutine.resume();
				});
			}
			else
			{
				coroutine.resume();
			}
		}

		T await_resume()
		{
			auto&& promise = coroutine.promise();

			if (auto&& exception = promise.exception)
			{
				std::rethrow_exception(exception);
			}
			return std::move(promise.value);
		}

		// For direct result access
		T get()
		{
			if (!coroutine.done())
			{
				coroutine.resume();
			}
			return await_resume();
		}

	public:
		Task(handle_type handle)
			: coroutine(handle)
		{
		}

		Task(const Task&) = delete;
		Task(Task&& task) noexcept
			: coroutine(std::exchange(task.coroutine, nullptr))
		{
		}

		~Task()
		{
			if (coroutine)
			{
				coroutine.destroy();
			}
		}
	};

	template<typename T>
	struct Task<T>::promise_type
	{
		T value;
		std::exception_ptr exception;
		std::coroutine_handle<> continuation;
		
		auto get_return_object()
		{
			return Task{ handle_type::from_promise(*this) };
		}

		auto initial_suspend()
		{
			return std::suspend_always{};
		}

		auto final_suspend() noexcept
		{
			struct Awaiter 
			{
				std::coroutine_handle<> continuation;
				bool await_ready() noexcept 
				{ 
					return !continuation;
				}
				void await_suspend(std::coroutine_handle<>) noexcept
				{
					if (continuation)
					{
						continuation.resume();
					}
				}
				void await_resume() noexcept 
				{
				}
			};
			return Awaiter{ continuation };
		}

		void return_value(T value) 
		{
			value = std::move(value);

			if (continuation)
			{
				continuation.resume();
			}
		}

		void unhandled_exception()
		{
			exception = std::current_exception();
		}
	};

	template<>
	struct Task<void>::promise_type
	{
		std::exception_ptr exception;
		std::coroutine_handle<> continuation;

		auto get_return_object() 
		{ 
			return Task{ handle_type::from_promise(*this) }; 
		}
		
		auto initial_suspend() 
		{ 
			return std::suspend_always{};
		}

		auto final_suspend() noexcept
		{
			struct Awaiter
			{
				std::coroutine_handle<> continuation;
				bool await_ready() noexcept
				{
					return !continuation;
				}
				void await_suspend(std::coroutine_handle<>) noexcept
				{
					if (continuation)
					{
						continuation.resume();
					}
				}
				void await_resume() noexcept
				{
				}
			};
			return Awaiter{ continuation };
		}

		void return_void() 
		{
			if (continuation)
			{
				continuation.resume();
			}
		}

		void unhandled_exception() 
		{ 
			exception = std::current_exception(); 
		}
	};
}

#endif
