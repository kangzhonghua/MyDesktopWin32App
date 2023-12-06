// HelloWold.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;
#include <string>
#include <future>

void remote_query(uint32_t query_index, std::function<void(uint32_t)>&& callback) {
    std::thread t(
        [query_index, callback = std::move(callback)]() {
            std::this_thread::sleep_for(4s);
            std::cout << std::to_string(query_index) + " times query: Hello, world!" << std::endl;
            callback(query_index);
        });
    t.detach();
}

void remote_callback(uint32_t query_index) {
    remote_query(++query_index, remote_callback);
}

int main() {
    remote_query(1, remote_callback);
    while (true)
    {
        std::this_thread::sleep_for(1s);
        std::cout << "main thread doing other things..." << std::endl;
    }
    return 0;
}