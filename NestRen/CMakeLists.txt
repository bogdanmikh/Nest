project(NestRen)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE src/*.hpp include/*.hpp)

# NestRen target
add_library(NestRen STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(NestRen PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(NestRen PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Dependencies - GLM
target_link_libraries(NestRen glm)
target_include_directories(NestRen PUBLIC ${VENDOR_DIRECTORY}/glm)

# Dependencies - stb_image
target_link_libraries(NestRen stb_image)
target_include_directories(NestRen PRIVATE ${VENDOR_DIRECTORY}/stb_image)

# Dependencies - GLAD
target_link_libraries(NestRen glad)
target_include_directories(NestRen PUBLIC ${VENDOR_DIRECTORY}/glad/include)

target_link_libraries(NestRen Nest)