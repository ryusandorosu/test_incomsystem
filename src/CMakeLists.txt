cmake_minimum_required(VERSION 3.22)
project(PostgreSQLProxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)

add_executable(PostgreSQLProxy main.cpp proxy_server.cpp logger.cpp)
