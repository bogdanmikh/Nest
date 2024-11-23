project(Example7)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example7 main.cpp)

# Dependencies - Rain
target_link_libraries(Example7 Rain)