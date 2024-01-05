set(SOURCES
        main.cpp

        Nest/include/Nest/Window/Key.hpp
        Nest/include/Nest/Window/Window.hpp

        Nest/include/Nest/NestRenderer/Shader.hpp
        Nest/include/Nest/NestRenderer/VertexBuffer.hpp
        Nest/include/Nest/NestRenderer/IndexBuffer.hpp
        Nest/include/Nest/NestRenderer/VertexArray.hpp
        Nest/include/Nest/NestRenderer/Renderer.hpp
        Nest/include/Nest/NestObjects/Shapes/Cube.hpp
        Nest/include/Nest/NestObjects/Camera.hpp

        Nest/src/Nest/Window/Window.cpp
        Nest/src/Nest/NestRenderer/Shader.cpp
        Nest/src/Nest/NestRenderer/VertexBuffer.cpp
        Nest/src/Nest/NestRenderer/Renderer.cpp
        Nest/src/Nest/NestRenderer/IndexBuffer.cpp
        Nest/src/Nest/NestRenderer/VertexArray.cpp
        Nest/src/Nest/NestRenderer/VertexArray.cpp
        Nest/src/Nest/NestRenderer/Texture.cpp

        Nest/src/Nest/NestObjects/Shapes/Cube.cpp
        Nest/src/Nest/NestObjects/Camera.cpp

        Examples/HelloTexture/src/Game.hpp
        Examples/HelloTexture/src/Game.cpp
        PARENT_SCOPE
)