cmake_minimum_required(VERSION 3.15)
project(GameServer LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)

add_definitions("-g")

add_subdirectory(server)
