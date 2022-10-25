cmake_minimum_required(VERSION 3.21)
project(Y86_64Processor C)

set(CMAKE_C_STANDARD 99)

add_executable(Y86_64Processor main.c utils.h utils.c)
