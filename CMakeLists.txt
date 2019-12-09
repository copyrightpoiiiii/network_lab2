cmake_minimum_required(VERSION 3.15)
project(network_lab2)

set(CMAKE_CXX_STANDARD 17)

link_directories(/Users/zhangqi/CLionProjects/network_lab2)

link_libraries(libnetsim.a)

aux_source_directory(. DIRS_SRCS)

add_executable(network_lab2 ${DIRS_SRCS})

target_link_libraries(network_lab2 libnetsim.a)