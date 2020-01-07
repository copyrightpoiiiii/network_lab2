cmake_minimum_required(VERSION 3.15)
project(network_lab2)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-fPIC)

#link_directories(./)

INCLUDE_DIRECTORIES(./header)

aux_source_directory(./SR DIRS_SRCS_SR)

aux_source_directory(./TCP DIRS_SRCS_TCP)

aux_source_directory(./GBN DIRS_SRCS_GBN)

aux_source_directory(./Wait DIRS_SRCS_Wait)

add_executable(network_lab2 main.cpp ${DIRS_SRCS_SR} ${DIRS_SRCS_GBN} ${DIRS_SRCS_Wait} ${DIRS_SRCS_TCP})

FIND_LIBRARY(NETSIM_LIB libnetsim.a ./)

TARGET_LINK_LIBRARIES(network_lab2 ${NETSIM_LIB} -lpthread)