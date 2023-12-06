#include <iostream>
#include <thread>
#include <coroutine>
#include <future>
#include <chrono>
#include <functional>

struct Result {
    struct promise_type {
        Result get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

std::coroutine_handle<> coroutine_handle;

struct AWaitableObject
{
    AWaitableObject() {}

    bool await_ready() const { return false; }

    int await_resume() { return 0; }

    void await_suspend(std::coroutine_handle<> handle) {
        coroutine_handle = handle;
    }

};


Result CoroutineFunction()
{
    std::cout << "start coroutine\n";
    int ret = co_await AWaitableObject();
    std::cout << "finish coroutine\n";
}



int main()
{
    std::cout << "start \n";
    auto coro = CoroutineFunction();
    std::cout << "coroutine co_await\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    coroutine_handle.resume();

    return 0;
}