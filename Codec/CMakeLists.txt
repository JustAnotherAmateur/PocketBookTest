cmake_minimum_required(VERSION 3.14)

project(Codec LANGUAGES CXX)

add_library(Codec SHARED
  Codec_global.h
  codec.cpp
  codec.h
  rawimagedata.h
)

#set_property(TARGET Codec PROPERTY IMPORTED_LOCATION "${Codec_BINARY_DIR}/libCodec.dll")
#set_property(TARGET Codec PROPERTY IMPORTED_IMPLIB "${Codec_BINARY_DIR}/libCodec.dll.a")

target_compile_definitions(Codec PRIVATE CODEC_LIBRARY)
