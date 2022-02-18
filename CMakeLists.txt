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
include_directories(./src/ip)
include_directories(./src/icmp)
include_directories(./src/sock)
include_directories(./src/dst)
include_directories(./src/arp)
add_executable(user_level_ip  ./src/tuntap/tuntap.c  ./src/tuntap/tuntap.h
        ./src/utils/utils.c ./src/utils/utils.h ./src/route/route.c
        ./inc/test/tinytest.h  ./inc/list_t/list.h ./src/netdev/netdev.c
        ./src/ethernet/ethernet.c ./src/skbuff/skbuff.c src/main.c src/ip/ip_input.c src/ip/ip_input.h src/ip/ip_info.c
        src/ip/ip_info.h src/icmp/icmpv4.c src/icmp/icmpv4.h src/sock/sock.c src/sock/sock.h src/ip/ip_output.c
        src/ip/ip_output.h src/dst/dst.c src/dst/dst.h src/arp/arp.c src/arp/arp.h src/ip/ip_fragment.c src/ip/ip_fragment.h src/timer/time_wheel_timer.h src/timer/time_wheel_timer.c)
target_link_libraries(${PROJECT_NAME}   pthread)