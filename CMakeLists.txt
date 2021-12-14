cmake_minimum_required(VERSION 2.8.12)
project(test C)

set(CMAKE_C_STANDARD 11)

include_directories(./src/tuntap)
include_directories(./src/utils)
include_directories(./src/test)
add_executable(test  ./src/tuntap/tuntap.c  ./src/tuntap/tuntap.h ./src/utils/utils.c ./src/utils/utils.h ./src/test/tinytest.h)
