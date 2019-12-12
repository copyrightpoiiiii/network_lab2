cmake_minimum_required(VERSION 3.15)
project(network_lab2)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-fPIC)

#link_directories(/home/zhangqi/CLionProjects/network_lab2)

INCLUDE_DIRECTORIES(/home/zhangqi/CLionProjects/network_lab2/header)

aux_source_directory(/home/zhangqi/CLionProjects/network_lab2/SR DIRS_SRCS_SR)

aux_source_directory(/home/zhangqi/CLionProjects/network_lab2/TCP DIRS_SRCS_TCP)

aux_source_directory(/home/zhangqi/CLionProjects/network_lab2/GBN DIRS_SRCS_GBN)

aux_source_directory(/home/zhangqi/CLionProjects/network_lab2/Wait DIRS_SRCS_Wait)

add_executable(network_lab2 main.cpp ${DIRS_SRCS_SR} ${DIRS_SRCS_GBN} ${DIRS_SRCS_Wait} ${DIRS_SRCS_TCP})

FIND_LIBRARY(NETSIM_LIB libnetsim.a /home/zhangqi/CLionProjects/network_lab2)

TARGET_LINK_LIBRARIES(network_lab2 ${NETSIM_LIB} -lpthread)