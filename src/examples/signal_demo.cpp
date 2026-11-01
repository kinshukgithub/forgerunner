#include <csignal>
#include <iostream>
#include <unistd.h>

volatile sig_atomic_t shutdownRequested = 0;

void handleSignal(int signal)
{
    if (signal == SIGINT || signal == SIGTERM) {
        shutdownRequested = 1;
    }
}

int main()
{

    //This is all stadnard signal handling code, which is used to handle the SIGINT and SIGTERM signals. When the program receives either of these signals, it sets the shutdownRequested flag to true, which will cause the main loop to exit and the program to terminate gracefully.
    struct sigaction action{};

    action.sa_handler = handleSignal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, nullptr) == -1) {
        std::cerr << "Failed to install SIGINT handler\n";
        return 1;
    }

    if (sigaction(SIGTERM, &action, nullptr) == -1) {
        std::cerr << "Failed to install SIGTERM handler\n";
        return 1;
    }

    std::cout << "Process running. Press Ctrl+C.\n";

    while (!shutdownRequested) {
        pause();
    }

    std::cout << "Shutdown requested\n";

    return 0;
}