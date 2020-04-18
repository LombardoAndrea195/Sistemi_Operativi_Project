cmake_minimum_required(VERSION 3.12)
project(Sistemi_Operativi_Lombardo C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Client client.c help.h)
add_executable(Server server.c help.h functionServer.h)

