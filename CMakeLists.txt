cmake_minimum_required(VERSION 3.22)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

include_directories(./src)

add_executable(webserver
        src/main.cpp
        )

