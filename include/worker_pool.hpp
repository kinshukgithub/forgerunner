#pragma once

#include <cstddef>
#include <thread>
#include <vector>

#include "job.hpp"
#include "job_queue.hpp"

class WorkerPool {
public:
    explicit WorkerPool(std::size_t workerCount);

    ~WorkerPool();

    bool submit(const Job& job);
    void shutdown();

private:
    void workerLoop(std::size_t workerId);
    void executeJob(const Job& job);

    JobQueue queue_;
    std::vector<std::thread> workers_;
};