#pragma once

#include <condition_variable>

namespace bv {
class Latch {
public:
    Latch(const int count) : count(count) {}

    void countDown() {
        std::lock_guard lk(countMutex);
        count--;
        if (count < 1)
            cv.notify_all();
    }

    void wait() {
        std::unique_lock<std::mutex> lk(countMutex);

        cv.wait(lk, [this]() {
            return count < 1;
        });
    }

private:
    std::condition_variable cv;
    int count;
    std::mutex countMutex;
};
}
