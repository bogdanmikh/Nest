#version 330 core

layout (location = 0) in vec2 aPos;

out vec4 Color;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    Color = vec4(1.0, 0.0, 0.2, 1.0);
}
