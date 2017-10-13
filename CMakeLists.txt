cmake_minimum_required(VERSION 3.8)
project(OptimizedSudoku)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Sudoku.h
        Sudoku.cpp
        test_sudoku.cpp)

add_executable(OptimizedSudoku ${SOURCE_FILES})