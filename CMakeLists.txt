cmake_minimum_required(VERSION 3.22)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

include_directories(./src)

set(RUST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/cgi/src/*.rs")
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/path_to_rust_project/target/release/cgi"
    COMMAND cargo build --release
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/cgi
    DEPENDS ${RUST_SRC}
)
