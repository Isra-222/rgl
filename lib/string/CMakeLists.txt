add_library(rglString STATIC
	format.cpp
	string.cpp
)
target_include_directories(rglString PUBLIC "${CMAKE_SOURCE_DIR}")