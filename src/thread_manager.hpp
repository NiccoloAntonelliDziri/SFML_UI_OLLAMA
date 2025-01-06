#pragma once

#include <future>
#include <iostream>
#include <thread>

template <typename ResultType>
class ThreadManager {
    public:
    ThreadManager() : isResultReady(true) {
        this->future = this->promise.get_future();
    }
    ~ThreadManager() { this->thread.join(); }

    // Pass function and arguments to start thread
    // Return value of function is stored in promise
    // with type ResultType (class template parameter)
    template <typename F, typename... Args>
    void start(F &&f, Args &&...args) {
        this->isResultReady = false;

        // If return type is void, different lambda function
        if constexpr (std::is_same_v<ResultType, void>) {
            this->thread = std::thread([this, f, args...]() {
                f(args...);
                this->promise.set_value();
                this->isResultReady = true;
            });
        } else {
            // Initialize thread with lambda function
            this->thread = std::thread(
                [this, f, args...]() { this->promise.set_value(f(args...)); });
        }
    };

    // Get result from promise
    inline ResultType getResult() { return this->future.get(); }

    inline bool isReady() { return this->isResultReady; }

    // Wait for thread to finish and reset promise
    inline void reset() {
        this->thread.join();
        this->promise = std::promise<ResultType>();
        this->future = this->promise.get_future();
    }

    private:
    std::thread thread;
    std::promise<ResultType> promise;
    std::future<ResultType> future;
    std::atomic<bool> isResultReady;
};
