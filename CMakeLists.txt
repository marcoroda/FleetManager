cmake_minimum_required(VERSION 3.15)
project(FleetManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/g++-9)

# enable cache system
include(cmake/Cache.cmake)

# Set up some extra Conan dependencies based on our needs before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")

if (CPP_STARTER_USE_IMGUI)
    set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} imgui-sfml/2.1@bincrafters/stable)
#    set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} imgui/1.80)
endif ()

include(cmake/Conan.cmake)
run_conan()

# wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include("${wxWidgets_USE_FILE}")

# Mongo
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)


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
        ${wxWidgets_LIBRARIES}
        CONAN_PKG::imgui-sfml
        mongo::bsoncxx_shared
        mongo::mongocxx_shared)

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