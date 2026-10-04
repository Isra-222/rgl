cmake_minimum_required(VERSION 4.4.3)
project(rgl CXX ASM) # Regular Library

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wno-unused-command-line-argument")
add_subdirectory(lib/Asm)

add_compile_options(
    -O3
    -march=native

    -fdiagnostics-color=always 
    -Wextra 
    -Wall 
    -Werror
    -fno-rtti
    -fno-exceptions 
    -fno-stack-protector 
    -fno-unwind-tables 
    -fno-asynchronous-unwind-tables

    -Wno-nullability-completeness
    
)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(
        -nostdinc
        -nostdinc++
        -ffreestanding
    )
    
    add_link_options(
        -nostdlib
        -nodefaultlibs
        -T ${CMAKE_CURRENT_SOURCE_DIR}/lib/os/linux/linker.ld
    )
endif()

if(WIN32)
    add_compile_definitions(RGL_NOT_OVERRIDE_GLOBAL_NEW)
endif()

include_directories(rgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(lib)

set(rgl_obj STATIC
    $<TARGET_OBJECTS:rglCore> 
    $<TARGET_OBJECTS:rglIo> 
    $<TARGET_OBJECTS:rglString>
    $<TARGET_OBJECTS:rglExtra>
    $<TARGET_OBJECTS:rglOs>
    $<TARGET_OBJECTS:rglMem>
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND rgl_obj
        $<TARGET_OBJECTS:rglNostd>
        $<TARGET_OBJECTS:rglAsm>
    )
endif()

add_library(rgl STATIC ${rgl_obj})

# choose the benchmark you want to try
add_executable(rgl_test benchmark/main.cpp) #example main.cpp

target_link_libraries(rgl_test PRIVATE rgl)