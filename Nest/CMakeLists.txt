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

if (DESKTOP)
    list(APPEND SOURCES "src/Platform/EntryPoint/Desktop/main.cpp")
elseif (PLATFORM_IOS)
    list(APPEND SOURCES "src/Platform/EntryPoint/iOS/main.mm")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

if(PLATFORM_DESKTOP)
    file(GLOB_RECURSE PLATFORM_INCLUDE src/Platform/WindowImpl/GlfwWindow/*.hpp src/Platform/EventsImpl/GlfwEvents/*.hpp)
    file(GLOB_RECURSE PLATFORM_SOURCES src/Platform/WindowImpl/GlfwWindow/*.cpp src/Platform/EventsImpl/GlfwEvents/*.cpp)
    list(APPEND INCLUDE ${PLATFORM_INCLUDE})
    list(APPEND SOURCES ${PLATFORM_SOURCES})
elseif(PLATFORM_ANDROID)
    file(GLOB_RECURSE PLATFORM_INCLUDE src/Platform/WindowImpl/AndroidWindow/*.hpp src/Platform/EventsImpl/AndroidEvents/*.hpp)
    file(GLOB_RECURSE PLATFORM_SOURCES src/Platform/WindowImpl/AndroidWindow/*.cpp src/Platform/EventsImpl/AndroidEvents/*.cpp)
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
elseif (PLATFORM_ANDROID)
    target_link_libraries(Nest glfm)
    target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/glfm/include)

    target_include_directories(Nest PUBLIC ${PROJECT_SOURCE_DIR}/src/Platform/WindowImpl/AndroidWindow)
    target_include_directories(Nest PUBLIC ${PROJECT_SOURCE_DIR}/src/Platform/EventsImpl/AndroidEvents)
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

if (PLATFORM_MACOS)
    # Dependencies - nfd
    target_link_libraries(Nest nfd)
endif ()

# Dependencies - enet
target_link_libraries(Nest enet_static)
target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/enet)

# Dependencies - miniaudio
target_link_libraries(Nest miniaudio)

# Dependencies - pugixml
target_link_libraries(Nest pugixml)
target_include_directories(Nest PUBLIC ${VENDOR_DIRECTORY}/pugixml/src)

# Dependencies - assimp
if (USE_ASSIMP)
    target_link_libraries(Nest assimp)
    target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/assimp/include)
endif ()