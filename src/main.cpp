#include <iostream>

#include "job.hpp"
#include "signal_manager.hpp"
#include "worker_pool.hpp"

int main()
{
    SignalManager signals;

    signals.blockSignalsForCurrentThread();

    WorkerPool pool(3);

    signals.start([&pool](int signal) {
        if (signal == SIGINT) {
            std::cout << "Received SIGINT\n";
        }
        else if (signal == SIGTERM) {
            std::cout << "Received SIGTERM\n";
        }

        std::cout << "Shutting down ForgeRunner...\n";

        pool.shutdown();
    });

    Job job1;
    job1.id = 1;
    job1.command = "hostname";

    Job job2;
    job2.id = 2;
    job2.command = "pwd";

    Job job3;
    job3.id = 3;
    job3.command = "ls";

    pool.submit(job1);
    pool.submit(job2);
    pool.submit(job3);

    std::cout << "ForgeRunner running\n";
    std::cout << "Press Ctrl+C to stop\n";

    signals.join();

    return 0;
}