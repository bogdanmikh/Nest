project(Example6)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example6 main.cpp)

# Dependencies - Rain
target_link_libraries(Example6 Rain)