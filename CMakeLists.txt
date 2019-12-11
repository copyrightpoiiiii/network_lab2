cmake_minimum_required(VERSION 3.15)
project(network_lab2)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-fPIC)

link_directories(/home/zhangqi/CLionProjects/network_lab2)

aux_source_directory(. DIRS_SRCS)

add_executable(network_lab2 ${DIRS_SRCS} )

FIND_LIBRARY(NETSIM_LIB libnetsim.a /home/zhangqi/CLionProjects/network_lab2)

TARGET_LINK_LIBRARIES(network_lab2  ${NETSIM_LIB})