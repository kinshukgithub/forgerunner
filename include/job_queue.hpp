#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

#include "job.hpp"

class JobQueue {
public:
    bool push(const Job& job);
    bool pop(Job& job);

    void shutdown();

private:
    std::queue<Job> jobs_;

    std::mutex mutex_;
    std::condition_variable condition_;

    bool stopping_{false};
};