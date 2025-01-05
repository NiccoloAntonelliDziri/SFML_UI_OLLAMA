#pragma once

#include <future>
#include <iostream>
#include <thread>

template <typename ResultType>
class ThreadManager {
    public:
    ThreadManager() { this->future = this->promise.get_future(); }
    ~ThreadManager() { this->thread.join(); }

    // Pass function and arguments to start thread
    // Return value of function is stored in promise
    // with type ResultType (class template parameter)
    template <typename F, typename... Args>
    void start(F &&f, Args &&...args) {
        std::cout << "Type of f: " << typeid(f).name() << std::endl;

        // If return type is void, different lambda function
        if constexpr (std::is_same_v<ResultType, void>) {
            this->thread = std::thread([this, f, args...]() {
                f(args...);
                this->promise.set_value();
            });
        } else {
            // Initialize thread with lambda function
            this->thread = std::thread(
                [this, f, args...]() { this->promise.set_value(f(args...)); });
        }
    };

    // Get result from promise
    ResultType getResult() { return this->future.get(); }

    private:
    std::thread thread;
    std::promise<ResultType> promise;
    std::future<ResultType> future;
};
