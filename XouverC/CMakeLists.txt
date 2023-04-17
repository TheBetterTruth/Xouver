cmake_minimum_required (VERSION 3.0)

add_executable (xouverc "main.cpp" "lexing/token.h" "parsing/ast.h"  "lexing/lexer.cpp" "common.cpp" "common.h" "exception.h" "exception.cpp"  "lexing/lexer.h"  "parsing/parser.cpp" "parsing/parser.h" "codegen/function.h" "codegen/type.h" "codegen/var.h" "codegen/iscope.h" "codegen/scope.cpp" "codegen/scope.h" "codegen/funcscope.h" "codegen/funcscope.cpp" "codegen/class.cpp" "codegen/class.h" "parsing/ast.cpp" "codegen/constant.h")
add_custom_command(TARGET xouverc PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_CURRENT_SOURCE_DIR}/test/ $<TARGET_FILE_DIR:xouverc>)

target_include_directories (xouverc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${PROJECT_SOURCE_DIR}/Common)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xouverc PROPERTY CXX_STANDARD 20)
endif()