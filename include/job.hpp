#pragma once

#include<string>
#include<sys/types.h>

enum class JobState{
    Queued,
    Running,
    Succeeded,
    Failed
};

struct Job{
    int id;
    pid_t pid;
    std::string command;
    JobState state;
    int exitCode;
};