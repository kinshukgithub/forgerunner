cmake_minimum_required(VERSION 3.16)

project(ForgeRunner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(forgerunner
    src/main.cpp
    src/job_queue.cpp
    src/worker_pool.cpp
)

target_include_directories(forgerunner PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(forgerunner PRIVATE Threads::Threads)
