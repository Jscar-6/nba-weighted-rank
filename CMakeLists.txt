cmake_minimum_required(VERSION 4.3)
project(nba_weighted_rank)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(nba_weighted_rank
        src/CSVParser.cpp
        src/CSVParser.h
        src/main.cpp
        src/MaxHeap.cpp
        src/MaxHeap.h
        src/QuickSelect.cpp
        src/QuickSelect.h
        src/Performance.h)
