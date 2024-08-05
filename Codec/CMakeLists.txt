cmake_minimum_required(VERSION 3.14)

project(Codec LANGUAGES CXX)

add_library(Codec SHARED
  Codec_global.h
  codec.cpp
  codec.h
  rawimagedata.h
)

target_compile_definitions(Codec PRIVATE CODEC_LIBRARY)
