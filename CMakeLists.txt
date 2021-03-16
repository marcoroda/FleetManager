cmake_minimum_required(VERSION 3.15)
project(FleetManager)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=google-*,cppcoreguidelines-*")

add_library(fleetmanager_lib
	STATIC
	src/utils.cpp
	src/Van.cpp)
target_include_directories(fleetmanager_lib
	PUBLIC
	include)

add_executable(fleetmanager
	main.cpp)
target_link_libraries(fleetmanager
	PRIVATE
	fleetmanager_lib)

