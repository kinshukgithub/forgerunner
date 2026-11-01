#include "job_queue.hpp"

bool JobQueue::push(const Job& job)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (stopping_) {
            return false;
        }

        jobs_.push(job);
    }

    condition_.notify_one();

    return true;
}

bool JobQueue::pop(Job& job)
{
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(lock, [this] {
        return stopping_ || !jobs_.empty();
    });

    if (stopping_ && jobs_.empty()) {
        return false;
    }

    job = jobs_.front();
    jobs_.pop();

    return true;
}

void JobQueue::shutdown()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopping_ = true;
    }

    condition_.notify_all();
}