cmake_minimum_required(VERSION 3.27)

add_library(xpkg STATIC "xpkg.cpp" "main.cpp" "bytecode_stream.cpp")
target_include_directories(xpkg PUBLIC ${PROJECT_SOURCE_DIR}/Include)