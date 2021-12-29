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
//                    std::cout << "Starting wait...\n";

                    cv.wait(lk, [this]() {
//                        std::cout << "Awakened...\n";
                        return kill || !tasks.empty();
                    });

                    if (kill)
                        break;

//                    std::cout << "Popping task...\n";

                    auto task = tasks.front();
                    tasks.pop();

                    lk.unlock();

                    task();

                    lk.lock();

//                    std::cout << "Finished task...\n";
                }
            }));
        }
    }

    void addTask(const std::function<void()>& task) {
//        std::cout << "Adding task...\n";
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
