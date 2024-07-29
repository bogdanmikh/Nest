cmake_minimum_required(VERSION 3.27)
project(Rain)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(BUILD_EXAMPLES ON)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE src/*.hpp include/Rain/*.hpp)

# RapidJson target

add_subdirectory(Vendor/rapidjson)
add_subdirectory(Vendor/yaml-cpp)

add_library(Rain STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(Rain PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Rain PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(Rain PUBLIC RapidJSON)
target_include_directories(Rain PUBLIC Vendor/rapidjson/include)

target_link_libraries(Rain PUBLIC yaml-cpp)
target_include_directories(Rain PUBLIC Vendor/yaml-cpp/include)

# Examples

if(BUILD_EXAMPLES)
    add_subdirectory(Examples/Example1)
    add_subdirectory(Examples/Example2)
    add_subdirectory(Examples/Example3)
    add_subdirectory(Examples/Example4)
    add_subdirectory(Examples/Example5)
    add_subdirectory(Examples/Example6)
    add_subdirectory(Examples/Example7)
    add_subdirectory(Examples/Example8)
    add_subdirectory(Examples/Example9)
    add_subdirectory(Examples/Example10)
    add_subdirectory(Examples/Example11)
endif ()