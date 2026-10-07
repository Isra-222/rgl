cmake_minimum_required(VERSION 4.4.3)
project(rgl CXX ASM) # Regular Library

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wno-unused-command-line-argument")

set(rgl_flags
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
set(rgl_link

)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND rgl_flags
        -nostdinc
        -nostdinc++
        -ffreestanding
    )
    
    list(APPEND rgl_link
        -nostdlib
        -nodefaultlibs
        -T ${CMAKE_CURRENT_SOURCE_DIR}/lib/os/linux/linker.ld
    )

    set(rgl_link "${rgl_link}" PARENT_SCOPE)
    set(rgl_flags "${rgl_flags}" PARENT_SCOPE)
elseif(WIN32)
    add_compile_definitions(RGL_NOT_OVERRIDE_GLOBAL_NEW)
endif()

add_compile_options(${rgl_flags})

include_directories(rgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(lib/Asm)
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
target_include_directories(rgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_options(rgl PRIVATE ${rgl_link})

#rgl_add (Only use with the executable)
function(rgl_add target_name)
    set(source_files "")
    set(libraries "")

    foreach(arg ${ARGN})
        if(arg MATCHES "\\.(cpp|c|cc|cxx|h|hpp|c\\+\\+)$")
            list(APPEND source_files ${arg})
        else()
            list(APPEND libraries ${arg})
        endif()
    endforeach()

    add_executable(${target_name} ${source_files})


    if(DEFINED rgl_flags)
        target_compile_options(${target_name} PRIVATE ${rgl_flags})
    endif()

    if(DEFINED rgl_link)
        message("METIENDO BANDERAS")
        target_link_options(${target_name} PRIVATE ${rgl_link})
    endif()

    target_link_libraries(${target_name} PRIVATE rgl ${libraries})
endfunction()

#rgl_submodule (Add dependecies)
function(rgl_submodule target_name)
    set(source_files "")
    set(libraries "")

    foreach(arg ${ARGN})
        if(arg MATCHES "\\.(cpp|c|cc|cxx|h|hpp|c\\+\\+)$")
            list(APPEND source_files ${arg})
        else()
            list(APPEND libraries ${arg})
        endif()
    endforeach()

    add_library(${target_name} STATIC ${source_files})

    if(DEFINED rgl_flags)
        target_compile_options(${target_name} PRIVATE ${rgl_flags})
    endif()

    target_link_libraries(${target_name} PRIVATE rgl ${libraries})
endfunction()

if(RGL_BUILD_TESTS)
    message(STATUS "RGL: Tests/Benchmarks enabled (RGL_BUILD_TESTS=ON)")
    rgl_add(rgl_test benchmark/main.cpp)
endif()