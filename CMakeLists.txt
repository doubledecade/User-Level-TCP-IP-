cmake_minimum_required(VERSION 2.8.12)
project(user_level_ip C)

set(CMAKE_C_STANDARD 11)

include_directories(./src/tuntap)
include_directories(./src/utils)
include_directories(./inc/test)
include_directories(./inc/list)
include_directories(./src/netdev)
add_executable(user_level_ip  ./src/tuntap/tuntap.c  ./src/tuntap/tuntap.h ./src/utils/utils.c ./src/utils/utils.h
        ./inc/test/tinytest.h src/tuntap/test/test_tuntap.c ./inc/list/list.h ./inc/list/list.c
        ./inc/list/list_iterator.c ./inc/list/list_node.c ./src/netdev/netdev.c)
