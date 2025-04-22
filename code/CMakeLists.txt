cmake_minimum_required(VERSION 3.16.3)

project(satlecture)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE RelDeb)

add_library(cadical STATIC IMPORTED)
set_target_properties(cadical PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libcadical.a)

find_package(LibArchive REQUIRED)
include_directories(${LibArchive_INCLUDE_DIRS})

add_executable(essential src/essential.cc)
target_link_libraries(essential cadical ${LibArchive_LIBRARIES})

add_executable(slursat src/slursat.cc)
target_link_libraries(slursat ${LibArchive_LIBRARIES})

add_executable(phole src/phole.cc)

add_executable(trailtest tests/trailtest.cc)

add_executable(pigeonhole src/pigeonhole.cc)
target_link_libraries(pigeonhole cadical)