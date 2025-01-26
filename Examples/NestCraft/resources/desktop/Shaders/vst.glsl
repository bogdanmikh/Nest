#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aLight;

uniform mat4 model;
uniform mat4 projViewMtx;

out vec2 TexCoord;
out float Light;

void main(){
    gl_Position = projViewMtx * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Light = aLight;
}