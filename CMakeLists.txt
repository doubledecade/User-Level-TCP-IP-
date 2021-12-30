cmake_minimum_required(VERSION 2.8.12)
project(user_level_ip C)

set(CMAKE_C_STANDARD 11)
include_directories(./src/tuntap)
include_directories(./src/utils)
include_directories(./inc/test)
include_directories(./inc/list_t)
include_directories(./src/netdev)
include_directories(./src/ethernet)
include_directories(./src/skbuff)
include_directories(./src/route)
include_directories(/usr/include)
add_executable(user_level_ip  ./src/tuntap/tuntap.c  ./src/tuntap/tuntap.h
        ./src/utils/utils.c ./src/utils/utils.h ./src/route/route.c
        ./inc/test/tinytest.h  ./inc/list_t/list.h ./src/netdev/netdev.c
        ./src/ethernet/ethernet.c ./src/skbuff/skbuff.c src/main.c)
target_link_libraries(${PROJECT_NAME}   pthread)