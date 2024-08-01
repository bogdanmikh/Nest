cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

project(Nest)

set(BIN_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(VENDOR_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Vendor)
set(NEST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Nest)
set(FOUNDATION_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Foundation)
set(NESTREN_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/NestRen)

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

include(cmake/add_app.cmake)

add_subdirectory(Vendor/glad)
add_subdirectory(Vendor/glfw)
add_subdirectory(Vendor/glm)
add_subdirectory(Vendor/imgui)
add_subdirectory(Vendor/spdlog)
add_subdirectory(Vendor/stb_image)
add_subdirectory(Rain)

add_subdirectory(Foundation)
add_subdirectory(NestRen)
add_subdirectory(Nest)

add_subdirectory(Examples)
