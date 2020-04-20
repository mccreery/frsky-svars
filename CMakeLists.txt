# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
cmake_minimum_required(VERSION 3.0)
include(GNUInstallDirs)

project(sport VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sport SHARED
    src/sport.cpp)

target_include_directories(sport PRIVATE src)
target_include_directories(sport PUBLIC include)

set_target_properties(sport PROPERTIES PUBLIC_HEADER
    include/sport.hpp)

set_target_properties(sport PROPERTIES VERSION ${PROJECT_VERSION})
#set_target_properties(sport PROPERTIES SOVERSION 1) # full release only

install(TARGETS sport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
