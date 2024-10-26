#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 Normal;

uniform mat4 projViewMtx;
uniform mat4 model;

void main() {
     gl_Position = projViewMtx * model * vec4(aPos, 1.0);
     TexCoords = aTexCoords;
     Normal = aNormal;
    //gl_Position = vec4(aPos, 1.0);
//    gl_Position = vec4(aPos, 1.0);
}