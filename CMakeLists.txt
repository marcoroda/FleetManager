cmake_minimum_required(VERSION 3.15)
project(FleetManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/g++-9)

# enable cache system
include(cmake/Cache.cmake)

# Set up some extra Conan dependencies based on our needs before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")

# Dear ImGui SFML
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} imgui-sfml/2.1@bincrafters/stable)
# wxWidgets
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} wxwidgets/3.1.4@bincrafters/stable)
# Mongo CXX drivers
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} mongo-cxx-driver/3.6.2)

include(cmake/Conan.cmake)
run_conan()

add_library(fleetmanager
        STATIC
        src/Utils.cpp
        src/Van.cpp
        src/DataAccess.cpp
        src/wx_app.cpp
        src/imgui_app.cpp
        src/Date.cpp)
target_include_directories(fleetmanager
        PUBLIC
        include)
target_link_libraries(fleetmanager
        PUBLIC
        CONAN_PKG::wxwidgets
        CONAN_PKG::imgui-sfml
        CONAN_PKG::mongo-cxx-driver
        CONAN_PKG::fmt
        CONAN_PKG::spdlog)

# wxWidgets GUI
add_executable(wx_fleetManager wx_main.cpp)
target_link_libraries(wx_fleetManager
        PRIVATE
        fleetmanager)

# ImGUI GUI
add_executable(imgui_fleetManager imgui_main.cpp)
target_link_libraries(imgui_fleetManager
        PRIVATE
        fleetmanager)

# ImGUI example
add_executable(demo_imgui demo_imgui.cpp)
target_link_libraries(demo_imgui
        PRIVATE
        fleetmanager)

if(ENABLE_TESTING)
    enable_testing()
    message("Building Tests.")
    add_subdirectory(test)
endif()