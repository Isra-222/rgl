add_library(rglString STATIC
	string.cpp
	stringMethods.cpp
	stringOperators.cpp
)
target_include_directories(rglString PUBLIC "${CMAKE_SOURCE_DIR}")