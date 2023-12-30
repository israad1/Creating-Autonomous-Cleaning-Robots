cmake_minimum_required(VERSION 3.16)

find_package(Catch2 3 REQUIRED)

IF (${Catch2_FOUND})
    MESSAGE(STATUS "Found Catch2")
ELSE (${Catch2_FOUND})
    MESSAGE(STATUS "Cound not find Catch2. Check to make sure its installed on your system!")
ENDIF (${Catch2_FOUND})

add_executable(robot_test robot_test.cpp)
add_executable(room_test room_test.cpp)
add_executable(fleet_test fleet_test.cpp)
add_executable(building_test building_test.cpp)
add_executable(technician_test technician_test.cpp)
add_executable(timer_test timer_test.cpp)
add_executable(manager_test manager_test.cpp)

target_link_libraries(robot_test
  PRIVATE
    Catch2::Catch2WithMain
    libClean
)

target_link_libraries(room_test
  PRIVATE
   Catch2::Catch2WithMain
   libClean
)

target_link_libraries(fleet_test
  PRIVATE
   Catch2::Catch2WithMain
   libClean
)

target_link_libraries(building_test
  PRIVATE
   Catch2::Catch2WithMain
   libClean
)

target_link_libraries(technician_test
  PRIVATE
   Catch2::Catch2WithMain
   libClean
)

target_link_libraries(timer_test
  PRIVATE
   Catch2::Catch2WithMain
   libClean
)

target_link_libraries(manager_test
  PRIVATE
   Catch2::Catch2WithMain
   libClean
)

target_compile_features(robot_test PRIVATE cxx_std_20)
target_compile_features(room_test PRIVATE cxx_std_20)
target_compile_features(fleet_test PRIVATE cxx_std_20)
target_compile_features(building_test PRIVATE cxx_std_20)
target_compile_features(technician_test PRIVATE cxx_std_20)
target_compile_features(timer_test PRIVATE cxx_std_20)
target_compile_features(manager_test PRIVATE cxx_std_20)