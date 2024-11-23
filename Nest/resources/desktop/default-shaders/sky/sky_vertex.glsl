#version 330 core

uniform mat4 model;
uniform mat4 projViewMtx;

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

void main()
{
    gl_Position = projViewMtx * model * vec4(aPos, 1.0);
    TexCoord = aPos;
}
