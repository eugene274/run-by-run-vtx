cmake_minimum_required(VERSION 3.20)
project(RunByRunVertex)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        ATTaskSkeleton
        URL      https://github.com/eugene274/AnalysisTreeTaskSkeleton/archive/refs/tags/0.9.2.tar.gz
)
FetchContent_MakeAvailable(ATTaskSkeleton)

add_executable(GetRunByRunVertex GetRunByRunVertex.cpp)
target_link_libraries(GetRunByRunVertex PRIVATE at_task_main)

