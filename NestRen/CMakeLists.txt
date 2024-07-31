project(NestRen)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/NestRen/*.cpp)
file(GLOB_RECURSE INCLUDE src/NestRen/*.hpp include/NestRen/*.hpp)

file(GLOB_RECURSE GLOB_RESULT_INCLUDE src/RendererImpl/OpenGL/*.hpp)
file(GLOB_RECURSE GLOB_RESULT_SOURCES src/RendererImpl/OpenGL/*.cpp)
list(APPEND INCLUDE ${GLOB_RESULT_INCLUDE})
list(APPEND SOURCES ${GLOB_RESULT_SOURCES})

# Other platform specific realizations
if (DESKTOP)
    list(APPEND INCLUDE "src/RendererImpl/Context/OpenGLContext.hpp")
    list(APPEND SOURCES "src/RendererImpl/Context/OpenGLContext.cpp")
elseif (PLATFORM_IOS)
    list(APPEND INCLUDE "src/RendererImpl/Context/GlesContext.hpp")
    list(APPEND SOURCES "src/RendererImpl/Context/GlesContext.mm")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

# NestRen target

add_library(NestRen STATIC "${SOURCES}" "${INCLUDE}")

target_compile_definitions(NestRen PUBLIC GLES_SILENCE_DEPRECATION)
target_include_directories(NestRen PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(NestRen PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (DESKTOP)
    # Dependencies - GLFW, only for Desktop :)
    target_link_libraries(NestRen glfw)
    target_include_directories(NestRen PRIVATE Vendor/glfw/include)
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

    target_link_libraries(NestRen
            ${UIKIT} ${METALKIT_LIBRARY} ${FOUNDATION} ${CFNETWORK} ${SYSTEMCONFIGURATION} ${QUARTZ_CORE} ${OPENGLES}
    )
endif ()

# Dependencies - Foundation
target_link_libraries(NestRen Foundation)
target_include_directories(NestRen PUBLIC ${FOUNDATION_DIRECTORY}/include)

# Dependencies - GLM
target_link_libraries(NestRen glm)
target_include_directories(NestRen PRIVATE ${VENDOR_DIRECTORY}/glm)

# Dependencies - GLAD
target_link_libraries(NestRen glad)
target_include_directories(NestRen PRIVATE ${VENDOR_DIRECTORY}/glad/include)

# Dependencies - stb_image
target_link_libraries(NestRen stb_image)
target_include_directories(NestRen PRIVATE ${VENDOR_DIRECTORY}/stb_image)