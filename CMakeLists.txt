# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
cmake_minimum_required(VERSION 3.0)
include(GNUInstallDirs)

project(sport VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic -Werror)
include_directories(include)
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

add_library(sport_obj OBJECT src/smartport.cpp)
set_property(TARGET sport_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

# Shared library
add_library(sport SHARED $<TARGET_OBJECTS:sport_obj>)
set_target_properties(sport PROPERTIES VERSION ${PROJECT_VERSION})
#set_target_properties(sport PROPERTIES SOVERSION 1) # full release only

install(TARGETS sport LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES include/frsky/sport.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/frsky)

# Static library
add_library(sport_static STATIC $<TARGET_OBJECTS:sport_obj>)
set_target_properties(sport_static PROPERTIES VERSION ${PROJECT_VERSION})
#set_target_properties(sport_static PROPERTIES SOVERSION 1) # full release only

install(TARGETS sport_static ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# CLI
add_executable(sport_cli src/cli.cpp)
target_link_libraries(sport_cli sport_static)
set_target_properties(sport_cli PROPERTIES VERSION ${PROJECT_VERSION})
