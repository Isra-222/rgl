#note: add support to difference windows and linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	add_subdirectory(linux)
endif()