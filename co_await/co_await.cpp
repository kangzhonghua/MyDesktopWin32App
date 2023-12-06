#include <coroutine>
#include <iostream>
#include <thread>


struct Job
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

	handle_type coro;

	Job(handle_type h) : coro(h)
	{
	}

	~Job()
	{
		if (coro) coro.destroy();
	}

	void start()
	{
		coro.resume();
	}

	struct promise_type
	{
		auto get_return_object()
		{
			return Job{ handle_type::from_promise(*this) };
		}

		std::suspend_always initial_suspend()
		{
			std::cout << "准备工作\n";
			return {};
		}

		std::suspend_always final_suspend() noexcept
		{
			std::cout << "执行工作\n";
			return {};
		}

		void return_void()
		{
		}

		void unhandled_exception()
		{
		}
	};
};

Job prepareJob()
{
	co_await std::suspend_never();
}


int main(int argc, char* argv[])
{
	std::cout << "工作之前\n";
	auto job = prepareJob();
	job.start();
	std::cout << "工作之后\n";
}
