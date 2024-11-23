project(Example1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example1 main.cpp)

# Dependencies - Rain
target_link_libraries(Example1 Rain)