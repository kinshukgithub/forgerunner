#include "worker_pool.hpp"

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

WorkerPool::WorkerPool(std::size_t workerCount)
{
    for (std::size_t i = 0; i < workerCount; ++i) {
        workers_.emplace_back([this, i] {
            workerLoop(i);
        });
    }
}

void WorkerPool::workerLoop(std::size_t workerId)
{
    while (true) {
        Job job;

        if (!queue_.pop(job)) {
            break;
        }

        std::cout << "Worker "
                  << workerId
                  << " picked job "
                  << job.id
                  << '\n';

        executeJob(job);
    }

    std::cout << "Worker "
              << workerId
              << " stopping\n";
}

bool WorkerPool::submit(const Job& job)
{
    return queue_.push(job);
}

void WorkerPool::executeJob(const Job& job)
{
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Failed to fork job "
                  << job.id
                  << '\n';
        return;
    }

    if (pid == 0) {
        execlp(
            job.command.c_str(),
            job.command.c_str(),
            nullptr
        );

        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        std::cout << "Job "
                  << job.id
                  << " exited with code "
                  << WEXITSTATUS(status)
                  << '\n';
    }
}

void WorkerPool::shutdown()
{
    queue_.shutdown();

    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

WorkerPool::~WorkerPool()
{
    shutdown();
}