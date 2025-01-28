#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 projViewMtx;

out vec3 TexCoord;

void main() {
    gl_Position = projViewMtx * model *  vec4(aPos * 20., 1.0);
    TexCoord = aPos;
}