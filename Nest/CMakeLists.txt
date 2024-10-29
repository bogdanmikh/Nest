project(Nest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

file(GLOB_RECURSE SOURCES src/Nest/*.cpp src/Platform/EntryPoint/*.cpp)
file(GLOB_RECURSE INCLUDE src/Nest/*.hpp include/*.hpp)

if(PLATFORM_DESKTOP)
    file(GLOB_RECURSE PLATFORM_INCLUDE src/Platform/WindowImpl/GlfwWindow/*.hpp src/Platform/EventsImpl/GlfwEvents/*.hpp)
    file(GLOB_RECURSE PLATFORM_SOURCES src/Platform/WindowImpl/GlfwWindow/*.cpp src/Platform/EventsImpl/GlfwEvents/*.cpp)
    list(APPEND INCLUDE ${PLATFORM_INCLUDE})
    list(APPEND SOURCES ${PLATFORM_SOURCES})
else()
    file(GLOB_RECURSE PLATFORM_INCLUDE src/Platform/WindowImpl/GlfmWindow/*.hpp src/Platform/EventsImpl/GlfmEvents/*.hpp)
    file(GLOB_RECURSE PLATFORM_SOURCES src/Platform/WindowImpl/GlfmWindow/*.cpp src/Platform/EventsImpl/GlfmEvents/*.cpp)
    list(APPEND INCLUDE ${PLATFORM_INCLUDE})
    list(APPEND SOURCES ${PLATFORM_SOURCES})
endif()

# Nest target
add_library(Nest STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(Nest PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Nest PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(PLATFORM_DESKTOP)
    # Dependencies - GLFW, only for Desktop :)
    target_link_libraries(Nest glfw)
    target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/glfw/include)
    target_include_directories(Nest PUBLIC ${PROJECT_SOURCE_DIR}/src/Platform/WindowImpl/GlfwWindow)
    target_include_directories(Nest PUBLIC ${PROJECT_SOURCE_DIR}/src/Platform/EventsImpl/GlfwEvents)
else()
    # Dependencies - GLFM, only for Phone :)
    target_link_libraries(Nest glfm)
    target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/glfm/include)

    target_include_directories(Nest PUBLIC ${PROJECT_SOURCE_DIR}/src/Platform/WindowImpl/GlfmWindow)
    target_include_directories(Nest PUBLIC ${PROJECT_SOURCE_DIR}/src/Platform/EventsImpl/GlfmEvents)
endif ()

# Dependencies - Foundation
target_link_libraries(Nest Foundation)

# Dependencies - Rain
target_link_libraries(Nest Rain)

# Dependencies - Bird
target_link_libraries(Nest Bird)

# Dependencies - GLM
target_link_libraries(Nest glm)
target_include_directories(Nest PUBLIC ${VENDOR_DIRECTORY}/glm)

# Dependencies - stb_image
target_link_libraries(Nest stb_image)
target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/stb_image)

# Dependencies - Dear ImGui
target_link_libraries(Nest imgui)
target_include_directories(Nest PUBLIC ${VENDOR_DIRECTORY}/imgui)

# Dependencies - Dear ImGui
target_link_libraries(Nest imgui)
target_include_directories(Nest PUBLIC ${VENDOR_DIRECTORY}/imgui)

# Dependencies - GLAD
target_link_libraries(Nest glad)
target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/glad/include)

# Dependencies - nfd
target_link_libraries(Nest nfd)

# Dependencies - enet
target_link_libraries(Nest enet_static)
target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/enet)

# Dependencies - assimp
if (USE_ASSIMP)
    target_link_libraries(Nest assimp)
    target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/assimp/include)
endif ()