add_library(rglIo STATIC
	filesystem.cpp
	ifstream.cpp
	istream.cpp
	ofstream.cpp
	ostream.cpp
)

target_include_directories(rglIo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")