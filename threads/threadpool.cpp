#include <iostream>
#include <chrono>

#include "threadpool.hpp"


int main() {
    std::cout << "Hello World" << std::endl;
    ThreadPool pool(4);

    std::vector<std::future<int>> results; // results able to execute call back

    for (int i = 0; i < 16; i++) {
        results.emplace_back(


                             // there the lambda has closure i, paremeter x, and a return value 
        pool.enqueue([i] (int x) {
                         // the acutal task job
                         std::cout << "hello" << x << ":"
                                   << i << std::endl;
                         std::this_thread::sleep_for(std::chrono::seconds(3));
                         return i * i; // 

                     }, 2 ) // here 2 is the argument passed to x
        );
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "------" << std::endl;
    for (auto& result : results) {
        // here wait for the call back 
        std::cout << result.get() << std::endl;
    }



}
