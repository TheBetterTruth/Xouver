cmake_minimum_required (VERSION 3.0)

add_library (xouverrt STATIC "logging.h" "runtime/runtime.cpp" "runtime/runtime.h" "class/xclass.h" "common.cpp" "common.h" "class/native/nativeclasses.h"  "object/object.h" "../Common/opcodes.h" "class/classmanager.cpp" "class/classmanager.h" "value/valuetype.h" "value/xvalue.h" "memory/memory.h" "memory/memory.cpp" "mapping/functionmap.cpp" "mapping/functionmap.h" "mapping/functionmap.cpp" "mapping/functionmap.h" "mapping/classmap.h" "mapping/classmap.cpp" "XNI.h" )
target_include_directories (xouverrt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" PUBLIC "${PROJECT_SOURCE_DIR}/Common")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xouverrt PROPERTY CXX_STANDARD 20)
endif()