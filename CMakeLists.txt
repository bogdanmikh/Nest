cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

project(Nest)

set(BIN_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(VENDOR_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Vendor)
set(NEST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Nest)
set(FOUNDATION_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Foundation)
set(BIRD_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bird)
set(ENET_STATIC ON)
set(USE_VULKAN OFF)
set(USE_ASSIMP ON)
set(PLATFORM_DESKTOP OFF)

if(PLATFORM_WIN32 OR PLATFORM_LINUX OR PLATFORM_MACOS)
    set(PLATFORM_DESKTOP ON)
endif()

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

include(cmake/add_app.cmake)

if(PLATFORM_DESKTOP)
    add_subdirectory(Vendor/glfw)
endif()

if (USE_VULKAN)
    find_package(Vulkan REQUIRED)

    if (${Vulkan_VERSION} VERSION_LESS "1.3.000" )
        message( FATAL_ERROR "Minimum required Vulkan version for C++ modules is 1.1.000. "
                "Found ${Vulkan_VERSION}."
        )
    endif()
endif ()

if (PLATFORM_MACOS)
    add_subdirectory(Vendor/nfd)
endif ()

if (USE_ASSIMP)
    add_subdirectory(Vendor/assimp)
endif ()

add_subdirectory(Vendor/glad)
add_subdirectory(Vendor/glm)
add_subdirectory(Vendor/imgui)
add_subdirectory(Vendor/spdlog)
add_subdirectory(Vendor/stb_image)
add_subdirectory(Vendor/dylib)
add_subdirectory(Vendor/enet)
add_subdirectory(Rain)

add_subdirectory(Foundation)
add_subdirectory(Bird)
add_subdirectory(Nest)

add_subdirectory(Examples)