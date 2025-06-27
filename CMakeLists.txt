cmake_minimum_required(VERSION 3.14)

project(SpatialTreeProject)

set(CMAKE_CXX_STANDARD 17)

# Enable testing
enable_testing()

# GoogleTest via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Add source files
file(GLOB_RECURSE PROJECT_SOURCES
    "src/Spacials/*.cpp"
    "src/Spacials/*.h"
    "src/Store/*.cpp"
    "src/Store/*.h"
    "src/TreeNodes/*.cpp"
    "src/TreeNodes/*.h"
)

add_library(rstartree ${PROJECT_SOURCES})
target_include_directories(rstartree PUBLIC 
    src
    src/Spacials
    src/Store
    src/TreeNodes
)

# Add test executable
add_executable(test_tree_interior_node src/tests/TestTreeInteriorNode.cpp)
target_link_libraries(test_tree_interior_node gtest_main rstartree)
add_executable(test_datapoint src/tests/TestDataPoint.cpp)
target_link_libraries(test_datapoint gtest_main rstartree)

# Register with CTest
add_test(NAME TreeInteriorNodeTest COMMAND test_tree_interior_node)
