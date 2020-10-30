cmake_minimum_required(VERSION 3.5)

# Set the project name
project (server)

set(THREADS_PREFER_PTHREAD_FLAG ON) 
find_package(Threads REQUIRED) 


# Add an executable
add_executable(server main.cpp server.cpp)

target_compile_options(server PRIVATE -Wall -Wno-long-long -pedantic)

target_link_libraries(server PRIVATE Threads::Threads)