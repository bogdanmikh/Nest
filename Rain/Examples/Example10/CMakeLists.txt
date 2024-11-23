project(Example10)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example10 main.cpp)

# Dependencies - Rain
target_link_libraries(Example10 Rain)