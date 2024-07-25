cmake_minimum_required(VERSION 3.5)

project(CodecTest LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(CodecTest tst_codectest.cpp)
add_test(NAME CodecTest COMMAND CodecTest)

target_link_libraries(CodecTest PRIVATE Qt${QT_VERSION_MAJOR}::Test Codec)
