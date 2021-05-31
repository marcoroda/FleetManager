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

# Mongo CXX drivers
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} mongo-cxx-driver/3.6.2)

# wxWidgets
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} wxwidgets/3.1.4@bincrafters/stable)

include(cmake/Conan.cmake)
run_conan()

add_library(fleetmanager_lib
        STATIC
        src/utils.cpp
        src/Van.cpp
        src/DataAccess.cpp
        src/wx_app.cpp
        src/imgui_app.cpp)
target_include_directories(fleetmanager_lib
        PUBLIC
        include)
target_link_libraries(fleetmanager_lib
        PUBLIC
        CONAN_PKG::wxwidgets
        CONAN_PKG::imgui-sfml
        CONAN_PKG::mongo-cxx-driver
        CONAN_PKG::fmt)

# wxWidgets GUI
add_executable(wx_fleetManager wx_main.cpp)
target_link_libraries(wx_fleetManager
        PRIVATE
        fleetmanager_lib)

# ImGUI GUI
add_executable(imgui_fleetManager imgui_main.cpp)
target_link_libraries(imgui_fleetManager
        PRIVATE
        fleetmanager_lib)

# ImGUI example
add_executable(demo_imgui demo_imgui.cpp)
target_link_libraries(demo_imgui
        PRIVATE
        fleetmanager_lib)