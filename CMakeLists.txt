cmake_minimum_required(VERSION 3.14)

project(PocketBookTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Codec)
add_subdirectory(CodecTest)
add_subdirectory(GUI)
