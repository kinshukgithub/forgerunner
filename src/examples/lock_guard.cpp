#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

constexpr int incrementsPerThread = 1000000;

int counter = 0;
std::mutex counterMutex;

//this one is more optimised, this creates a local counter for each thread and 
//then adds it to the global counter at the end, reducing the number of times the mutex is locked and unlocked.
//In this, there will be lock and unlock only once for each thread, which is more efficient.
void incrementCounter2()
{
    int localCounter = 0;

    for (int i = 0; i < incrementsPerThread; ++i) {
        ++localCounter;
    }

    std::lock_guard<std::mutex> lock(counterMutex);
    counter += localCounter;
}

//In this, there will be lock and unlock 1000000 times for each thread, which is inefficient.
void incrementCounter()
{
    for (int i = 0; i < incrementsPerThread; ++i) {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++counter;
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