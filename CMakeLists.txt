cmake_minimum_required(VERSION 3.20)
project(nba_weighted_rank)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nba_weighted_rank
        src/main.cpp
        src/CSVParser.cpp
        src/QuickSelect.cpp
        src/MaxHeap.cpp
)

target_include_directories(nba_weighted_rank PRIVATE src)