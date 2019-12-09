cmake_minimum_required(VERSION 3.15)
project(network_lab2)

set(CMAKE_CXX_STANDARD 17)

link_directories(/Users/zhangqi/CLionProjects/network_lab2)

aux_source_directory(. DIRS_SRCS)

add_executable(network_lab2 ${DIRS_SRCS} GoBackN_RdtReceiver.cpp GoBackN_RdtReceiver.h GoBackN_RdtSender.cpp GoBackN_RdtSender.h SelectRdtReceiver.cpp SelectRdtReceiver.h SelectRdtSender.cpp SelectRdtSender.h)

FIND_LIBRARY(NETSIM_LIB libnetsim.a /home/zhangqi/CLionProjects/network_lab2)

TARGET_LINK_LIBRARIES(network_lab2  ${NETSIM_LIB})