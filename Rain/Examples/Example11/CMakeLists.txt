project(Example11)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example11 main.cpp)

# Dependencies - Rain
target_link_libraries(Example11 Rain)