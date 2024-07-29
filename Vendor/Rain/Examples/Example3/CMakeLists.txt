project(Example3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example3 main.cpp)

# Dependencies - Rain
target_link_libraries(Example3 Rain)