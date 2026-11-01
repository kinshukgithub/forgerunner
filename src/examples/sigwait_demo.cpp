#include <csignal>
#include <iostream>
#include <pthread.h>
#include <thread>

void signalThread(sigset_t signalSet)
{
    std::cout << "Signal thread waiting...\n";

    int signalNumber = 0;

    int result = sigwait(&signalSet, &signalNumber);

    if (result != 0) {
        std::cerr << "sigwait failed\n";
        return;
    }

    if (signalNumber == SIGINT) {
        std::cout << "Received SIGINT\n";
    }
    else if (signalNumber == SIGTERM) {
        std::cout << "Received SIGTERM\n";
    }
}

int main()
{
    sigset_t signalSet;

    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGINT);
    sigaddset(&signalSet, SIGTERM);

    int result = pthread_sigmask(SIG_BLOCK, &signalSet, nullptr);

    if (result != 0) {
        std::cerr << "pthread_sigmask failed\n";
        return 1;
    }

    std::thread signalHandler(signalThread, signalSet);

    signalHandler.join();

    return 0;
}