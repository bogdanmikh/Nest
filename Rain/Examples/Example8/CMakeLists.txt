project(Example8)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example8 main.cpp)

# Dependencies - Rain
target_link_libraries(Example8 Rain)