#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <iostream>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>

class ThreadPool {
public:
    // explicit to prevent auto convertion
    explicit ThreadPool(size_t size);

    template<typename F, typename... Args> // a paremeter pack
    std::future<int> enqueue(F&& func, Args&& ... args);

    ~ThreadPool();

private:
    std::vector<std::thread> workers;

    std::queue<std::packaged_task<void()> > tasks;

    std::mutex thread_mutex;
    std::condition_variable condition;
    bool stop;
};

inline ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(thread_mutex);
        stop = true; // so that each thread will exit loop 
    }
    condition.notify_all();
    for (auto& worker : workers) {
        worker.join(); // just blocks until the thread execution is complete 
    }
}


inline ThreadPool::ThreadPool(size_t size) : stop(false) {
    for (size_t i = 0; i < size; ++i) {
        // emplace => don't need to create the object

        workers.emplace_back([this] () {
                                 // getting tasks from task queue
                                 while(true) { // always trying
                                     std::packaged_task<void()> task;
                                     {
                                         // unique_lock for conditional variable 
                                         std::unique_lock<std::mutex> workerLock(this->thread_mutex);

                                         // first wait the tasks queue
                                         auto l = [this] {
                                                      // wait unless 
                                                      return this->stop || !this->tasks.empty();
                                                  };

                                         this->condition.wait(workerLock, l);


                                         if (this->stop && this->tasks.empty()) {
                                             // stop and no tasks
                                             return;
                                         }

                                         // copy the whole task, 
                                         task = std::move(this->tasks.front());
                                         this->tasks.pop();
                                     }

                                     // so that we could run task out of locker 
                                     task();
                                 }
                             });


    }
}


template<typename F, typename... Args> 
inline std::future<int> ThreadPool::enqueue(F&& func, Args&& ... args) {
    // packaged_task pack return in its shared state within (future)
    std::packaged_task<int()> task(std::bind(std::forward<F>(func), std::forward<Args>(args)...));

    std::future<int> res = task.get_future();
    {
        std::unique_lock<std::mutex> lock(thread_mutex);
        if (stop) {
            //throw;
        }

        tasks.emplace(std::move(task));
    }

    // wake up on waiting  thread
    condition.notify_one();
    return res;
}

#endif
