cmake_minimum_required(VERSION 3.22)
project(PSQLProxyServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)

add_executable(PSQLProxyServer client.cpp connection.cpp socket.cpp logger.cpp main.cpp)

target_link_libraries(${PROJECT_NAME} pthread)