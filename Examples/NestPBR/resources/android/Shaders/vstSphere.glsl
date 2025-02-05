#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projViewMtx;

out vec2 TexCoord;
out vec3 Normal;
out vec3 Position;

void main() {
    vec4 pos = projViewMtx * model * vec4(aPos, 1.0);
    gl_Position = pos;
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
}