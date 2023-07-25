cmake_minimum_required (VERSION 3.0)

include (CheckTypeSize)
check_type_size ("int" INT)

if (HAS_INT)
if (INT == 4)

endif()
endif()

set (xpkgsrc "xpkg.cpp" "xpkg.h" "helpers.h" "codes.h")
add_executable (xpkg "main.cpp" ${xpkgsrc})
add_library (xpkg-obj OBJECT ${xpkgsrc})

set_property(TARGET xpkg-obj PROPERTY POSITION_INDEPENDENT_CODE TRUE)

add_library (xpkg-shared SHARED $<TARGET_OBJECTS:xpkg-obj>)
add_library (xpkg-static STATIC $<TARGET_OBJECTS:xpkg-obj>)