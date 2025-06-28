cmake_minimum_required(VERSION 3.14)

project(SpatialTreeProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


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
# TreeInteriorNode test
add_executable(test_tree_interior_node src/tests/TestTreeInteriorNode.cpp)
target_link_libraries(test_tree_interior_node gtest_main rstartree)
# DataPoint test
add_executable(test_datapoint src/tests/TestDataPoint.cpp)
target_link_libraries(test_datapoint gtest_main rstartree)
# Point test
add_executable(test_point src/tests/TestPoint.cpp)
target_link_libraries(test_point gtest_main rstartree)
# TreeLeafNode test
add_executable(test_tree_leaf_node src/tests/TestTreeLeafNode.cpp)
target_link_libraries(test_tree_leaf_node gtest_main rstartree)

# Register with CTest
add_test(NAME TreeInteriorNodeTest COMMAND test_tree_interior_node)
add_test(NAME DataPointTest COMMAND test_datapoint)
add_test(NAME PointTest COMMAND test_point)
add_test(NAME TreeLeafNodeTest COMMAND test_tree_leaf_node)
