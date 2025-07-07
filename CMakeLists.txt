cmake_minimum_required(VERSION 3.16)
project(MemoryAllocation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS specific flags
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")

# Include directories
include_directories(src/allocator)
include_directories(src/queue)

# Allocator library
add_library(fixed_allocator
    src/allocator/fixAlloc.cpp
)

# Main executable
add_executable(allocator_test main.cpp)
target_link_libraries(allocator_test fixed_allocator)
