#include<iostream>
#include<coroutine>
using namespace std;

struct future_type_int {
    struct promise_type;
    using co_handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        int ret_val;
        promise_type() {
            std::cout << "promise_type constructor" << std::endl;
        }
        ~promise_type() {
            std::cout << "promise_type destructor" << std::endl;
        }
        auto get_return_object() {
            std::cout << "get_return_object" << std::endl;
            return co_handle_type::from_promise(*this);
        }
        auto initial_suspend() {
            std::cout << "initial_suspend" << std::endl;
            return std::suspend_always();
        }
        auto final_suspend() noexcept(true) {
            std::cout << "final_suspend" << std::endl;
            return std::suspend_never();
        }
        void return_value(int val) {
            std::cout << "return_value : " << val << std::endl;
            ret_val = val;
        }
        void unhandled_exception() {
            std::cout << "unhandled_exception" << std::endl;
            std::terminate();
        }
        auto yield_value(int val) {
            std::cout << "yield_value : " << val << std::endl;
            ret_val = val;
            return std::suspend_always();
        }
    };
    future_type_int(co_handle_type co_handle) {
        std::cout << "future_type_int constructor" << std::endl;
        co_handle_ = co_handle;
    }
    ~future_type_int() {
        std::cout << "future_type_int destructor" << std::endl;
        co_handle_.destroy();
    }
    future_type_int(const future_type_int&) = delete;
    future_type_int(future_type_int&&) = delete;

    bool resume() {
        if (!co_handle_.done()) {
            co_handle_.resume();
        }
        return !co_handle_.done();
    }
    co_handle_type co_handle_;
};

future_type_int three_step_coroutine() {
    std::cout << "three_step_coroutine begin" << std::endl;
    co_yield 222;
    std::cout << "three_step_coroutine running" << std::endl;
    co_yield 333;
    std::cout << "three_step_coroutine end" << std::endl;
    co_return 444;
}

int main() {
    future_type_int future_obj = three_step_coroutine();

    std::cout << "=======calling first resume======" << std::endl;
    future_obj.resume();
    std::cout << "ret_val = " << future_obj.co_handle_.promise().ret_val << std::endl;

    std::cout << "=======calling second resume=====" << std::endl;
    future_obj.resume();
    std::cout << "ret_val = " << future_obj.co_handle_.promise().ret_val << std::endl;

    std::cout << "=======calling third resume======" << std::endl;
    future_obj.resume();
    std::cout << "ret_val = " << future_obj.co_handle_.promise().ret_val << std::endl;
    std::cout << "=======main end======" << std::endl;

    return 0;
}
