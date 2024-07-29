project(Nest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE src/*.hpp include/*.hpp)

# Nest target
add_library(Nest STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(Nest PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Nest PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Dependencies - GLM
target_link_libraries(Nest glm)
target_include_directories(Nest PUBLIC ${VENDOR_DIRECTORY}/glm)

# Dependencies - stb_image
target_link_libraries(Nest stb_image)
target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/stb_image)

# Dependencies - Dear ImGui
target_link_libraries(Nest imgui)
target_include_directories(Nest PUBLIC ${VENDOR_DIRECTORY}/imgui)

# Dependencies - GLFW, only for Desktop :)
target_link_libraries(Nest glfw)
target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/glfw/include)

# Dependencies - Dear ImGui
target_link_libraries(Nest imgui)
target_include_directories(Nest PUBLIC ${VENDOR_DIRECTORY}/imgui)

# Dependencies - GLAD
target_link_libraries(Nest glad)
target_include_directories(Nest PUBLIC ${VENDOR_DIRECTORY}/glad/include)

# Dependencies - spdlog
target_link_libraries(Nest spdlog)
target_include_directories(Nest PRIVATE ${VENDOR_DIRECTORY}/spdlog/include)