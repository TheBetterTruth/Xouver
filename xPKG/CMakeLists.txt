cmake_minimum_required (VERSION 3.0)

set (xpkgsrc "xpkg.cpp" "xpkg.h")
add_executable (xpkg "main.cpp" ${xpkgsrc})
add_library (xpkg-obj OBJECT ${xpkgsrc})

set_property(TARGET xpkg-obj PROPERTY POSITION_INDEPENDENT_CODE TRUE)

add_library (xpkg-shared SHARED $<TARGET_OBJECTS:xpkg-obj>)
add_library (xpkg-static STATIC $<TARGET_OBJECTS:xpkg-obj>)