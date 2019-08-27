#include <thread>
#include <iostream>
#include <mutex>

std::mutex mu;

void print_id() {
    std::unique_lock<std::mutex> l(mu);
    std::cout << "thread_id:" << std::this_thread::get_id() << std::endl;
}

int main() {
    std::thread t1(print_id);
    std::thread t2(print_id);

    t1.join();
    t2.join();
}
