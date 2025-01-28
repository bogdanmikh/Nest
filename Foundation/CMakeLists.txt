project(Foundation)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE src/*.hpp include/Foundation/*.hpp)

# Foundation target

add_library(Foundation STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(Foundation PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Foundation PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Dependencies - spdlog
target_link_libraries(Foundation spdlog)
target_include_directories(Foundation PRIVATE ${VENDOR_DIRECTORY}/spdlog/include)
