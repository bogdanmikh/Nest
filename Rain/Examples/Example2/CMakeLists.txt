project(Example2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example2 main.cpp)

# Dependencies - Rain
target_link_libraries(Example2 Rain)