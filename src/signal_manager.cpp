#include "signal_manager.hpp"

#include <iostream>
#include <pthread.h>

SignalManager::SignalManager()
{
    sigemptyset(&signalSet_);
    sigaddset(&signalSet_, SIGINT);
    sigaddset(&signalSet_, SIGTERM);
}

void SignalManager::blockSignalsForCurrentThread()
{
    int result = pthread_sigmask(
        SIG_BLOCK,
        &signalSet_,
        nullptr
    );

    if (result != 0) {
        throw std::runtime_error(
            "Failed to block signals"
        );
    }
}

void SignalManager::start(std::function<void(int)> handler)
{
    handler_ = std::move(handler);

    thread_ = std::thread([this] {
        signalLoop();
    });
}

void SignalManager::signalLoop()
{
    while (true) {
        int signalNumber = 0;

        int result = sigwait(
            &signalSet_,
            &signalNumber
        );

        if (result != 0) {
            std::cerr << "sigwait failed\n";
            continue;
        }

        handler_(signalNumber);

        if (signalNumber == SIGINT ||
            signalNumber == SIGTERM) {
            break;
        }
    }
}

void SignalManager::join()
{
    if (thread_.joinable()) {
        thread_.join();
    }
}