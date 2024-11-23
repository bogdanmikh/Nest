project(Example5)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example5 main.cpp)

# Dependencies - Rain
target_link_libraries(Example5 Rain)