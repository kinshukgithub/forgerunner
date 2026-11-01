#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include <unistd.h>
#include <sys/wait.h>

#include <chrono>

void worker(int id, int seconds)
{
    std::cout
        << "Worker "
        << id
        << " started\n";

    std::this_thread::sleep_for(
        std::chrono::seconds(seconds)
    );

    std::cout
        << "Worker "
        << id
        << " finished\n";
}

void runJob(int jobId, const std::string& command)
{
    std::cout << "Job " << jobId << " starting\n";

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork failed for job " << jobId << '\n';
        return;
    }

    if (pid == 0) {
        execlp(command.c_str(), command.c_str(), nullptr);
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        std::cout << "Job " << jobId
                  << " finished with exit code "
                  << WEXITSTATUS(status)
                  << '\n';
    }
}

int main()
{
    std::vector<std::thread> workers;

    workers.emplace_back(runJob, 1, "hostname");
    workers.emplace_back(runJob, 2, "ls");
    workers.emplace_back(runJob, 3, "pwd");

    for (auto& worker : workers) {
        worker.join();
    }

    // std::thread t1(worker, 1, 50);
    // std::thread t2(worker, 2, 3);
    // std::thread t3(worker, 3, 10);

    // t1.join();
    // t2.join();
    // t3.join();

    return 0;

    return 0;
}