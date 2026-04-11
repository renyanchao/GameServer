cmake_minimum_required(VERSION 3.15)
project(GameServer LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 强制使用 C++11，不允许降级
set(CMAKE_CXX_EXTENSIONS OFF)       # 禁用编译器特定扩展，使用纯标准
add_definitions("-g")


add_subdirectory(server)
