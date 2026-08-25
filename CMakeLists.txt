cmake_minimum_required(VERSION 3.10)
project(rgl CXX) # Regular Library

add_compile_options(
    -fdiagnostics-color=always 
    -Wextra 
    -Wall 
    -Werror
    -fno-rtti 
    -fno-exceptions 
    -fno-stack-protector 
    -fno-unwind-tables 
    -fno-asynchronous-unwind-tables
)

add_subdirectory(lib/core)
add_subdirectory(lib/io)
add_subdirectory(lib/string)

add_library(rgl STATIC 
    $<TARGET_OBJECTS:rglCore> 
    $<TARGET_OBJECTS:rglIo> 
    $<TARGET_OBJECTS:rglString>
)

#active the not redefinition of operator new:
# target_compile_definitions(rgl PUBLIC RGL_NOT_OVERRIDE_GLOBAL_NEW)

include_directories(rgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(rgl_test main.cpp)
target_link_libraries(rgl_test PRIVATE rgl)