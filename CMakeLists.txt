cmake_minimum_required(VERSION 3.25)

set(NameProject Nest)

project(${NameProject})

set(CMAKE_CXX_STANDARD 17)

add_executable(${NameProject} main.cpp
        Nest/include/Nest/Window/Window.cpp
        Nest/include/Nest/Window/Key.hpp
        Nest/include/Nest/Window/Window.hpp
        Nest/include/Nest/NestRenderer/Shader.cpp
        Nest/include/Nest/NestRenderer/Shader.hpp
        Nest/include/Nest/NestRenderer/VertexBuffer.cpp
        Nest/include/Nest/NestRenderer/VertexBuffer.hpp
        Nest/include/Nest/NestRenderer/Renderer.cpp
        Nest/include/Nest/NestRenderer/Renderer.hpp
        Nest/include/Nest/NestRenderer/IndexBuffer.cpp
        Nest/include/Nest/NestRenderer/VertexArray.cpp
        Nest/include/Nest/NestRenderer/VertexArray.cpp
        Nest/include/Nest/NestRenderer/Texture.cpp

        Examples/HelloTexture/src/Game.cpp
        Examples/HelloTexture/src/Game.hpp
        Nest/include/Nest/NestObjects/Shapes/Cube.hpp

        Nest/include/Nest/NestObjects/World.cpp
        Nest/include/Nest/NestObjects/World.hpp
        Nest/include/Nest/NestObjects/GameObject.hpp
        Nest/include/Nest/NestObjects/World.cpp
        Nest/include/Nest/NestObjects/Shapes/Cube.cpp
)

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

target_include_directories(${NameProject} PRIVATE Nest/include)

#bgfx
#add_subdirectory(Vendor/bgfx)
#
#target_link_libraries(${NameProject} bgfx)
#target_include_directories(${NameProject} PRIVATE Vendor/bgfx)


