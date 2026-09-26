cmake_minimum_required(VERSION 4.4.3)
project(rgl CXX ASM) # Regular Library

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wno-unused-command-line-argument")
add_subdirectory(lib/Asm)

option(RGL_NOT_OVERRIDE_GLOBAL_NEW "Prevent overriding global new/delete operators" OFF)

set(RGL_DEFAULT_STACK_SIZE       1048576 CACHE STRING "Default thread stack size in bytes (Default: 1MB)")
set(RGL_MAX_THREAD_DESTRUCTORS   64      CACHE STRING "Maximum destructors per thread")
set(RGL_MAX_DESTRUCTORS          256     CACHE STRING "Global maximum of destructors")
add_compile_options(
    -O3
    -march=native
    -flto=thin

    -fdiagnostics-color=always 
    -Wextra 
    -Wall 
    -Werror
    -fno-rtti
    -fno-exceptions 
    -fno-stack-protector 
    -fno-unwind-tables 
    -fno-asynchronous-unwind-tables

    -nostdinc
    -nostdinc++
    -ffreestanding

    -Wno-nullability-completeness
    
)

add_link_options(
    -nostdlib
    -nodefaultlibs
    -flto=thin
    -fno-use-linker-plugin
    -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
)

include_directories(rgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_definitions(
    RGL_DEFAULT_STACK_SIZE=${RGL_DEFAULT_STACK_SIZE}
    RGL_MAX_THREAD_DESTRUCTORS=${RGL_MAX_THREAD_DESTRUCTORS}
    RGL_MAX_DESTRUCTORS=${RGL_MAX_DESTRUCTORS}
)
if(RGL_NOT_OVERRIDE_GLOBAL_NEW)
    add_compile_definitions(RGL_NOT_OVERRIDE_GLOBAL_NEW)
endif()

add_subdirectory(lib)

add_library(rgl STATIC
    $<TARGET_OBJECTS:rglCore> 
    $<TARGET_OBJECTS:rglIo> 
    $<TARGET_OBJECTS:rglString>
    $<TARGET_OBJECTS:rglExtra>
    $<TARGET_OBJECTS:rglOs>
    $<TARGET_OBJECTS:rglNostd>
    $<TARGET_OBJECTS:rglAsm>
)

# choose the benchmark you want to try
add_executable(rgl_test benchmark/main.cpp) #example main.cpp
set_property(TARGET rgl_test PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

target_link_libraries(rgl_test PRIVATE rgl)