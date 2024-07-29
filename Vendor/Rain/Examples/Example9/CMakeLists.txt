project(Example9)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example9 main.cpp)

# Dependencies - Rain
target_link_libraries(Example9 Rain)