project(Example12)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example12 main.cpp)

# Dependencies - Rain
target_link_libraries(Example12 Rain)