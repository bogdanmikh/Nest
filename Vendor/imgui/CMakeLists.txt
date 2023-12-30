project(imgui)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(imgui STATIC ${SOURCES} ${HEADERS})
target_include_directories(imgui PRIVATE include)
