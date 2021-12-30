#pragma once

#include <functional>
#include <queue>
#include <memory>
#include <thread>
#include <iostream>

namespace bv {
class ThreadPool {
public:
    ThreadPool(const int numThreads) {
        threads.reserve(numThreads);
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(std::thread([this]() {
                std::unique_lock<std::mutex> lk(tasksMutex);
                for (;;) {
                    cv.wait(lk, [this]() {
                        return kill || !tasks.empty();
                    });

                    if (kill)
                        break;

                    auto task = tasks.front();
                    tasks.pop();

                    lk.unlock();

                    task();

                    lk.lock();
                }
            }));
        }
    }

    void enqueue(const std::function<void()>& task) {
        {
            std::lock_guard lk(tasksMutex);
            tasks.emplace(task);
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard lk(tasksMutex);
            kill = true;
        }
        cv.notify_all();
        for (auto& t : threads)
            t.join();
    }

private:
    std::queue<std::function<void()>> tasks;
    mutable std::mutex tasksMutex;

    bool kill = false;
    std::condition_variable cv;

    std::vector<std::thread> threads;
};
}
