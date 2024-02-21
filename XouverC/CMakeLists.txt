cmake_minimum_required(VERSION 3.27)

add_executable(xouverc "main.cpp" "lexer.cpp" "parser.cpp"     "code_geerator.cpp" "code_generator.h")

target_include_directories(xouverc PUBLIC ${PROJECT_SOURCE_DIR}/Include PUBLIC ${PROJECT_SOURCE_DIR}/XPKG)
target_link_libraries(xouverc PUBLIC xpkg)