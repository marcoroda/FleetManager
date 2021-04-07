cmake_minimum_required(VERSION 3.15)
project(FleetManager)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=google-*,cppcoreguidelines-*")

find_package(wxWidgets REQUIRED COMPONENTS core base)
include("${wxWidgets_USE_FILE}")

add_library(fleetmanager_lib
        STATIC
        src/utils.cpp
        src/Van.cpp
        src/app.cpp)
target_include_directories(fleetmanager_lib
        PUBLIC
        include)
target_link_libraries(fleetmanager_lib
        PRIVATE
        ${wxWidgets_LIBRARIES})

add_executable(fleetManager
        src/main.cpp)
target_link_libraries(fleetManager
        PRIVATE
        fleetmanager_lib)



