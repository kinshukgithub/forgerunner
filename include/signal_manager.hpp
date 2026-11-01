#pragma once

#include <csignal>
#include <functional>
#include <thread>

class SignalManager {
public:
    SignalManager();

    void blockSignalsForCurrentThread();

    void start(std::function<void(int)> handler);

    void join();

private:
    void signalLoop();

    sigset_t signalSet_{};
    std::function<void(int)> handler_;
    std::thread thread_;
};