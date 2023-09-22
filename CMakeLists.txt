cmake_minimum_required(VERSION 3.26)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

find_package(ODBC REQUIRED)

include_directories(${ODBC_INCLUDE_DIRS})
link_directories(${ODBC_LIBRARY_DIRS})

add_executable(lab1 main.cpp
        config_reader.h
        models/Pharmacy.h)

target_link_libraries(lab1 ${ODBC_LIBRARIES})