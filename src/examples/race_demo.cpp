#include <iostream>
#include <thread>
#include <vector>

int completedJobs = 0;

void runJob(int id)
{
    std::cout << "Job " << id << " running\n";

    ++completedJobs;
}

int main()
{
    std::vector<std::thread> workers;

    for (int i = 0; i < 1000; ++i) {
        workers.emplace_back(runJob, i);
    }

    for (auto& worker : workers) {
        worker.join();
    }

    std::cout << "Completed jobs: " << completedJobs << '\n';

    return 0;
}