cmake_minimum_required(VERSION 2.8.12)
project(test C)

set(CMAKE_C_STANDARD 11)

include_directories(./src/tuntap)

add_executable(test  ./src/tuntap/tuntap.c  ./src/tuntap/tuntap.h)
