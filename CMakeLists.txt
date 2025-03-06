cmake_minimum_required(VERSION 3.10)
project(BigAssTerrainSim)

# Use Clang as the compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER /usr/bin/clang CACHE FILEPATH "C Compiler")
set(CMAKE_CXX_COMPILER /usr/bin/clang++ CACHE FILEPATH "C++ Compiler")

# Find GDAL
find_package(GDAL REQUIRED)
# Explicitly set the GDAL include directory
include_directories(${GDAL_INCLUDE_DIRS})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
# Add Thrust include directory
include_directories(/usr/include)

# Define THRUST to use the CPU backend
add_definitions(-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_CPP)

# Automatically find all .cpp files in `src/`
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Define RasterLoader as a library
add_library(RasterLoader STATIC src/raster_loader.cpp)
target_include_directories(RasterLoader PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(RasterLoader PRIVATE GDAL::GDAL)

# Add executable and link the RasterLoader library
add_executable(BigAssTerrainSim ${SRC_FILES})
target_link_libraries(BigAssTerrainSim PRIVATE RasterLoader)

# Enable more warnings for better code quality
target_compile_options(BigAssTerrainSim PRIVATE -Wall -Wextra -Wpedantic)

# Enable testing framework
enable_testing()

# Add Google Test
add_subdirectory(external/gtest/googletest)

# Locate test files
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/*.cpp)

# Add executable for testing
add_executable(RasterLoaderTests ${TEST_SOURCES})
target_link_libraries(RasterLoaderTests PRIVATE RasterLoader gtest_main)

# Enable GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(RasterLoaderTests)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=modernize-*,performance-*")
add_custom_target(
    format
    COMMAND clang-format -i ${SRC_FILES} ${HEADER_FILES}
    COMMENT "Formatting source files with clang-format"
)

