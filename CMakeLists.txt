cmake_minimum_required(VERSION 3.16)
project(libClean)

include(dependencies.cmake)

add_library(libClean
  src/room.cpp
  src/manager.cpp
  src/robot.cpp
  src/fleet.cpp
  src/building.cpp
  src/technician.cpp
  src/timer.cpp
)

target_include_directories(libClean
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(libClean
  fmt::fmt
)

add_subdirectory(app)
add_subdirectory(tests)