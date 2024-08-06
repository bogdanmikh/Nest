project(Bird)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/Bird/*.cpp)
file(GLOB_RECURSE INCLUDE src/Bird/*.hpp include/Bird/*.hpp)

file(GLOB_RECURSE GLOB_RESULT_INCLUDE src/Platform/RendererImpl/OpenGL/*.hpp src/Platform/RendererImpl/Vulkan/*.hpp)
file(GLOB_RECURSE GLOB_RESULT_SOURCES src/Platform/RendererImpl/OpenGL/*.cpp src/Platform/RendererImpl/Vulkan/*.cpp)
list(APPEND INCLUDE ${GLOB_RESULT_INCLUDE})
list(APPEND SOURCES ${GLOB_RESULT_SOURCES})

# Other platform specific realizations
if (DESKTOP)
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/OpenGLContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/OpenGLContext.cpp")
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/VulkanContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/VulkanContext.cpp")
elseif (PLATFORM_IOS)
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/GlesContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/GlesContext.mm")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

# Bird target

add_library(Bird STATIC "${SOURCES}" "${INCLUDE}")

target_compile_definitions(Bird PUBLIC GLES_SILENCE_DEPRECATION)
target_include_directories(Bird PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Bird PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (DESKTOP)
    # Dependencies - GLFW, only for Desktop :)
    target_link_libraries(Bird glfw Vulkan::Vulkan)
    target_include_directories(Bird PRIVATE Vendor/glfw/include)
elseif (PLATFORM_IOS)
    # add_definitions("-x objective-c++")
    # Locate system libraries on iOS
    find_library(OPENGLES OpenGLES)
    find_library(METALKIT_LIBRARY MetalKit)
    find_library(QUARTZ_CORE QuartzCore)
    find_library(FOUNDATION Foundation)
    find_library(MOBILECORESERVICES MobileCoreServices)
    find_library(CFNETWORK CFNetwork)
    find_library(SYSTEMCONFIGURATION SystemConfiguration)

    target_link_libraries(Bird
            ${UIKIT} ${METALKIT_LIBRARY} ${FOUNDATION} ${CFNETWORK} ${SYSTEMCONFIGURATION} ${QUARTZ_CORE} ${OPENGLES}
    )
endif ()

# Dependencies - Foundation
target_link_libraries(Bird Foundation)
target_include_directories(Bird PUBLIC ${FOUNDATION_DIRECTORY}/include)

# Dependencies - GLM
target_link_libraries(Bird glm)
target_include_directories(Bird PRIVATE ${VENDOR_DIRECTORY}/glm)

# Dependencies - GLAD
target_link_libraries(Bird glad)
target_include_directories(Bird PRIVATE ${VENDOR_DIRECTORY}/glad/include)