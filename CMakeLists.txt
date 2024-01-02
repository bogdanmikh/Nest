cmake_minimum_required(VERSION 3.25)

set(NameProject Nest)

project(NameProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(${NameProject} main.cpp
        Nest/src/Window/Window.cpp
        Nest/src/Window/Key.hpp
        Nest/src/Window/Window.hpp
        Nest/src/NestRenderer/Shader.cpp
        Nest/src/NestRenderer/Shader.hpp
        Nest/src/NestRenderer/VertexBuffer.cpp
        Nest/src/NestRenderer/VertexBuffer.hpp
        Nest/src/NestRenderer/Renderer.cpp
        Nest/src/NestRenderer/Renderer.hpp
        Nest/include/Nest.hpp
        Nest/src/NestRenderer/IndexBuffer.cpp
        Nest/src/NestRenderer/VertexArray.cpp
        Nest/src/NestRenderer/VertexArray.cpp
        Nest/src/NestRenderer/Texture.cpp

        Examples/HelloTexture/src/HelloTexture.cpp
        Examples/HelloTexture/src/HelloTexture.hpp
        Examples/HelloSquare/src/HelloSquare.hpp
        Examples/HelloSquare/src/HelloSquare.cpp Nest/src/Entity/Level.hpp)

#------------------------------------------------------------------------------------------------------------------#
# GLFW
add_subdirectory(Vendor/glfw)

target_link_libraries(${NameProject} glfw)
target_include_directories(${NameProject} PRIVATE Vendor/glfw/include)

# GLAD
add_subdirectory(Vendor/glad)

target_link_libraries(${NameProject} glad)
target_include_directories(${NameProject} PRIVATE Vendor/glad/include)

# GLM
add_subdirectory(Vendor/glm)

target_link_libraries(${NameProject} glm)
target_include_directories(${NameProject} PRIVATE Vendor/glm)

# STB_IMAGE
add_subdirectory(Vendor/stb_image)

target_link_libraries(${NameProject} stb_image)
target_include_directories(${NameProject} PRIVATE Vendor/stb_image)

# imgui
add_subdirectory(Vendor/imgui)

target_link_libraries(${NameProject} imgui)
target_include_directories(${NameProject} PRIVATE Vendor/imgui)
