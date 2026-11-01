#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

constexpr int incrementsPerThread = 1000000;

int counter = 0;
std::mutex counterMutex;

void incrementCounter()
{
    for (int i = 0; i < incrementsPerThread; ++i) {
        counterMutex.lock();

        ++counter;

        counterMutex.unlock();
    }
}

int main()
{
    constexpr int threadCount = 8;

    std::vector<std::thread> threads;

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(incrementCounter);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Expected: "
              << threadCount * incrementsPerThread
              << '\n';

    std::cout << "Actual:   "
              << counter
              << '\n';

    return 0;
}