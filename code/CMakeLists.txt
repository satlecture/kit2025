cmake_minimum_required(VERSION 3.16.3)

project(satlecture)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE RelDeb) # change to "Debug" for full debugging symbols

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

add_executable(graphcol src/graphcol.cc)
target_link_libraries(graphcol cadical)

add_executable(sdvstp src/sdvstp/sdvstp.cc src/util/pb/cardinality_encoding.cc src/util/pb/enc_adder.cc)
add_executable(sdvstp_gen src/sdvstp/sdvstp_gen.cc) # for generating benchmark problems
target_link_libraries(sdvstp cadical)

add_executable(sdvstpp src/sdvstp/sdvstpp.cc src/util/pb/cardinality_encoding.cc src/util/pb/enc_adder.cc)
target_link_libraries(sdvstpp cadical)
