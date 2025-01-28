project(Bird)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/Bird/*.cpp)
file(GLOB_RECURSE INCLUDE src/Bird/*.hpp include/Bird/*.hpp)

file(GLOB_RECURSE GLOB_RESULT_INCLUDE src/Platform/RendererImpl/OpenGL/*.hpp)
file(GLOB_RECURSE GLOB_RESULT_SOURCES src/Platform/RendererImpl/OpenGL/*.cpp)

if (PLATFORM_DESKTOP AND USE_VULKAN)
    file(GLOB_RECURSE VULKAN_INCLUDE src/Platform/RendererImpl/Vulkan/*.hpp)
    file(GLOB_RECURSE VULKAN_SOURCE src/Platform/RendererImpl/Vulkan/*.cpp)
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/VulkanContext.hpp")
endif ()

list(APPEND INCLUDE ${GLOB_RESULT_INCLUDE})
list(APPEND SOURCES ${GLOB_RESULT_SOURCES})

# Other platform specific realizations
if (PLATFORM_DESKTOP)
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/OpenGLContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/OpenGLContext.cpp")
elseif (PLATFORM_IOS)
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/GlesContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/GlesContext.mm")
elseif (PLATFORM_ANDROID)
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/AndroidContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/AndroidContext.cpp")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

# Bird target

add_library(Bird STATIC "${SOURCES}" "${INCLUDE}")

target_compile_definitions(Bird PUBLIC GLES_SILENCE_DEPRECATION)
target_include_directories(Bird PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Bird PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (PLATFORM_DESKTOP)
    # Dependencies - GLFW, only for Desktop :)
    if (USE_VULKAN)
        target_link_libraries(Bird glfw Vulkan::Vulkan)
    else ()
        target_link_libraries(Bird glfw)
    endif ()
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
elseif (PLATFORM_ANDROID)
    find_package(game-activity)
    target_link_libraries(Bird android game-activity::game-activity EGL GLESv3 jnigraphics)
endif ()

# Dependencies - Foundation
target_link_libraries(Bird Foundation)
target_include_directories(Bird PUBLIC ${FOUNDATION_DIRECTORY}/include)

# Dependencies - GLM
target_link_libraries(Bird glm)
target_include_directories(Bird PRIVATE ${VENDOR_DIRECTORY}/glm)

if (PLATFORM_DESKTOP)
    # Dependencies - GLAD
    target_link_libraries(Bird glad)
    target_include_directories(Bird PUBLIC ${VENDOR_DIRECTORY}/glad/include)
endif ()