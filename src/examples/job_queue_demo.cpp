#include <chrono>
#include <iostream>
#include <thread>

#include "job.hpp"
#include "job_queue.hpp"

void worker(JobQueue& queue)
{
    std::cout << "Worker waiting for job...\n";

    Job job = queue.pop();

    std::cout << "Worker received job "
              << job.id
              << ": "
              << job.command
              << '\n';
}

int main()
{
    JobQueue queue;

    std::thread workerThread(worker, std::ref(queue));

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Submitting job...\n";

    Job job;
    job.id = 1;
    job.command = "ls";

    queue.push(job);

    workerThread.join();

    return 0;
}