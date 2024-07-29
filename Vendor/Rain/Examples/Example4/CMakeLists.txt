project(Example4)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example4 main.cpp)

# Dependencies - Rain
target_link_libraries(Example4 Rain)