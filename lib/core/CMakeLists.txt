add_library(rglCore STATIC
	error.cpp
)
target_include_directories(rglCore PUBLIC "${CMAKE_SOURCE_DIR}")