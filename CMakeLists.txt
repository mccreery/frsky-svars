# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
cmake_minimum_required(VERSION 3.0)
add_subdirectory(serial)

include(GNUInstallDirs)

project(sport VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic -Werror)
include_directories(include)
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

# Shared library
add_library(sport SHARED src/smartport.cpp)
target_link_libraries(sport serial)
set_target_properties(sport PROPERTIES VERSION ${PROJECT_VERSION})
#set_target_properties(sport PROPERTIES SOVERSION 1) # full release only

install(TARGETS sport LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES include/frsky/sport.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/frsky)

# CLI
add_executable(sport_cli src/cli.cpp)
target_link_libraries(sport_cli sport)
target_link_libraries(sport_cli readline)
set_target_properties(sport_cli PROPERTIES VERSION ${PROJECT_VERSION})

enable_testing()
include(GoogleTest)

add_executable(sport_tests src/tests.cpp)
target_link_libraries(sport_tests gtest_main)
target_link_libraries(sport_tests sport)
gtest_discover_tests(sport_tests)
